#include <Scene.h>
#include <SceneObject.h>
#include <MaterialProperties.h>
#include <Ray.h>
#include <chrono>
#include <gtx/string_cast.hpp>
#include <iostream>
#include <iomanip>

namespace rayTracer {

    namespace {

        void displayTimeTaken(int timeInMilliSeconds) {
            int timeInMinutes = (timeInMilliSeconds / 1000) / 60;
            float restSeconds = (float(timeInMilliSeconds) / 1000.0f) - float(timeInMinutes) * 60;

            std::cout << "Time taken: ";
            if (timeInMinutes > 0)
                std::cout << timeInMinutes << "min and ";
            std::cout << restSeconds << "s " << std::endl;
        }

    } // anonymous namespace

    Scene::Scene()
        : renderSettings(RenderSettings())
    {
        std::random_device rd;
        gen = new std::mt19937(rd());
        dis = new std::uniform_real_distribution<float>(0, 1);
    }

    ///----------------------------------------------

    Scene::~Scene()
    {
        delete gen;
        delete dis;
    }

    ///----------------------------------------------

    void Scene::render(const std::string cameraName, const RenderSettings& settings)
    {
        if (sceneCameras.find(cameraName) == sceneCameras.end())
        {
            std::cout << "The given camera name '" << cameraName << "' does not exist. Exiting.." << std::endl;
            return;
        }
            
        std::shared_ptr<Camera> camera = sceneCameras.at(cameraName);
        int pixelWidth = camera->getPixelWidth();
        int pixelHeight = camera->getPixelHeight();

        renderSettings = settings;

        // For randomness in the ray generation
        std::random_device rd;
        std::mt19937 genHalf(rd());
        std::uniform_real_distribution<float> disHalf(-0.5, 0.5);

        // For calculating time taken
        auto startTime = std::chrono::high_resolution_clock::now();

        // Calculate the pixel values by sending out rays into the scene
        int lastPercentageOutputted = -1;
        for (int i = 0; i < pixelHeight; i++)
        {
#pragma omp parallel for
            for (int j = 0; j < pixelWidth; j++) {
                glm::vec3 finalColor = glm::vec3(0.0f);
                for (int subSample = 0; subSample < renderSettings.numSubSamplesPerPixel; ++subSample)
                {
                    std::shared_ptr<Ray> newRay = camera->createCameraRay(j, pixelHeight - i - 1, disHalf(genHalf), disHalf(genHalf));
                    finalColor += traceRay(newRay);
                }

                camera->setPixelValue(i, j, finalColor / float(renderSettings.numSubSamplesPerPixel));
            }

            // Print out progress every x% done
            int percentageDone = int(float(i + 1) / float(pixelHeight) * 100);
            if (percentageDone % renderSettings.outputProgressEveryXPercent == 0 
                && percentageDone != lastPercentageOutputted)
            {
                std::cout << "[" << std::setw(3) << percentageDone << "%] ";
                lastPercentageOutputted = percentageDone;
                if (percentageDone == 0)
                    std::cout << "Starting off";
                else if (percentageDone == 50)
                    std::cout << "Halfway there!";
                else if (percentageDone == 100)
                    std::cout << "Done!";
                std::cout << std::endl;
            }
        }

        // Generate the image from the pixel values
        camera->generateImage();

        // Calculate time taken
        auto endTime = std::chrono::high_resolution_clock::now();
        displayTimeTaken(int(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()));
    }

    void Scene::addSphere(float radius, glm::vec3 centerPosition, MaterialPtr material, bool emissive) {
        std::shared_ptr<Sphere> newSphere = std::make_shared<Sphere>(radius, centerPosition, material);
        sceneObjects.push_back(newSphere);
        if (emissive)
            emissiveObjectIndices.push_back(int(sceneObjects.size()) - 1);
    }

    ///----------------------------------------------

    void Scene::addBox(glm::mat4x4 transform, MaterialPtr material, bool emissive ) {
        std::shared_ptr<VertexObject> newBox = VertexObject::createBox(transform, material);
        sceneObjects.push_back(newBox);
        if (emissive)
            emissiveObjectIndices.push_back(int(sceneObjects.size()) - 1);
    }

    ///----------------------------------------------

    void Scene::addPlane(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, MaterialPtr material, bool emissive) {
        std::shared_ptr<VertexObject> newPlane = VertexObject::createPlane(p0, p1, p2, p3, material);
        sceneObjects.push_back(newPlane);
        if (emissive)
            emissiveObjectIndices.push_back(int(sceneObjects.size()) - 1);
    }

    ///----------------------------------------------

    void Scene::addCamera(std::shared_ptr<Camera> camera)
    {
        sceneCameras[camera->getName()] = camera;
    }

    ///----------------------------------------------

    std::shared_ptr<Scene> Scene::createDefaultScene() {
        std::shared_ptr<Scene> defaultScene = std::make_shared<Scene>();

        // Create cornell box
        MaterialPtr diffuseRed = std::make_shared<LambertianMaterial>(glm::vec3(1.f, 0.f,0.f));
        MaterialPtr diffuseWhite = std::make_shared<LambertianMaterial>(glm::vec3(1.f, 1.f,1.f));
        MaterialPtr diffuseGreen = std::make_shared<LambertianMaterial>(glm::vec3(0.f, 1.f,0.f));

        glm::vec3 p0 = glm::vec3(-1.5f, -1.f, -1.f);
        glm::vec3 p1 = glm::vec3(1.5f, -1.f, -1.f);
        glm::vec3 p2 = glm::vec3(1.5f, 1.f, -1.f);
        glm::vec3 p3 = glm::vec3(-1.5f, 1.f, -1.f);
        glm::vec3 p4 = glm::vec3(-1.5f, -1.f, 4.f);
        glm::vec3 p5 = glm::vec3(1.5f, -1.f, 4.f);
        glm::vec3 p6 = glm::vec3(1.5f, 1.f, 4.f);
        glm::vec3 p7 = glm::vec3(-1.5f, 1.f, 4.f);

        defaultScene->addPlane(p0, p1, p2, p3, diffuseWhite); // Back wall
        defaultScene->addPlane(p4, p7, p6, p5, diffuseWhite); // Front wall
        defaultScene->addPlane(p0, p3, p7, p4, diffuseRed); // Left wall
        defaultScene->addPlane(p1, p5, p6, p2, diffuseGreen); // Right wall
        defaultScene->addPlane(p2, p6, p7, p3, diffuseWhite); // Roof
        defaultScene->addPlane(p0, p4, p5, p1, diffuseWhite); // Floor

        // Add objects inside cornell box
        MaterialPtr diffuseMagenta = std::make_shared<LambertianMaterial>(glm::vec3(1.f, 0.f,1.f));
        MaterialPtr diffuseCyan = std::make_shared<LambertianMaterial>(glm::vec3(0.f, 1.f,1.f));

        glm::mat4x4 boxTransform = glm::mat4x4(1.0f);
        boxTransform = glm::translate(boxTransform, glm::vec3(-0.8, -0.3, -0.3));
        boxTransform = glm::rotate(boxTransform, glm::pi<float>() / 3, glm::vec3(0, 1, 0));
        boxTransform = glm::scale(boxTransform, glm::vec3(0.8,1.3,0.8));
        defaultScene->addBox(boxTransform, diffuseMagenta);

        MaterialPtr mirror = std::make_shared<PerfectMirrorMaterial>();
        defaultScene->addSphere(0.3f, glm::vec3(0.4f, -0.5f, 0.0f), mirror);

        // Add a light source
        MaterialPtr emissiveWhite = std::make_shared<EmissiveMaterial>(glm::vec3(1.f, 1.f, 1.f), 30.f);
        glm::vec3 lightP1 = glm::vec3(0.35f, 0.99f, -0.35f);
        glm::vec3 lightP2 = glm::vec3(-0.35f, 0.99f, -0.35f);
        glm::vec3 lightP3 = glm::vec3(0.35f, 0.99f, 0.35f);
        glm::vec3 lightP4 = glm::vec3(-0.35f, 0.99f, 0.35f);
        defaultScene->addPlane(lightP1, lightP3, lightP4, lightP2, emissiveWhite, true);

        return defaultScene;
    }

    ///----------------------------------------------

    glm::vec3 Scene::traceRay(std::shared_ptr<Ray> ray) const
    {
        // Something's gone wrong, we can't find any intersections within the scene..
        if (!findClosestIntersection(ray))
            return glm::vec3(0.0f);

        // For gathering all the indirect lighting in the scene
        glm::vec3 indirectLight = glm::vec3(0.0f);

        // Generate a reflected ray
        std::shared_ptr<Ray> reflectedRay = ray->generateReflectedRay(gen, dis);

        // Send out the reflected ray if we hit the randomized threshold or if the object
        // we have hit is not a diffuse object or a light source
        float randomNum = (*dis)(*gen);
        if (ray->hitsEmissiveObject())
            indirectLight = ray->getValueOfBRDF(reflectedRay);
        else if (!ray->hitsDiffuseObject() || randomNum < renderSettings.russianRouletteCoefficient)
            indirectLight += traceRay(reflectedRay) * ray->getValueOfBRDF(reflectedRay);;

        // Calculate direct lighting using shadow rays
        glm::vec3 directLight = glm::vec3(0.0f);
        if (ray->hitsDiffuseObject())
            directLight = calculateDirectLighting(ray);

        return glm::clamp(indirectLight + directLight, 0.0f, 1.0f);
    }

    ///----------------------------------------------

    bool Scene::findClosestIntersection(std::shared_ptr<Ray> currentRay) const {
        bool intersection = false;
        for(auto& sceneObject : sceneObjects){
            if(sceneObject->intersect(currentRay) && !intersection)
                intersection = true;
        }
        return intersection;
    }

    ///----------------------------------------------

    glm::vec3 Scene::calculateDirectLighting(const std::shared_ptr<Ray> ray) const
    {
        glm::vec3 allLightsContributions = glm::vec3(0.0);
        for (int index : emissiveObjectIndices)
        {
            glm::vec3 singleLightContribution = glm::vec3(0.0);
            std::shared_ptr<SceneObject> emissiveObject = sceneObjects[index];

            for (int i = 0; i < renderSettings.numShadowRays; i++)
            {
                // Generate a shadow ray from the point of intersection to a random point on the light source
                glm::vec3 randomPointOnEmissiveObject = emissiveObject->getRandomPointOnObject(ray, gen, dis);
                std::shared_ptr<Ray> shadowRay = ray->generateShadowRay(randomPointOnEmissiveObject);

                singleLightContribution += getShadowRayContribution(ray, shadowRay);
            }

            singleLightContribution *= (emissiveObject->radiance() * emissiveObject->area()) / float(renderSettings.numShadowRays);
            allLightsContributions += singleLightContribution;
        }

        return glm::clamp(allLightsContributions, 0.0f, 1.0f);
    }

    ///----------------------------------------------

    glm::vec3 Scene::getShadowRayContribution(const std::shared_ptr<Ray> originalRay, std::shared_ptr<Ray> shadowRay) const
    {
        // Get normalized shadow ray direction
        glm::vec3 shadowRayDirection = glm::normalize(shadowRay->getDirection());

        // Calculate angle between the shadow ray and the normal of the surface. If the angle is
        // more than 90 degrees the light should not be able to hit the surface, return black.
        float cosBeta = glm::dot(shadowRayDirection, originalRay->getIntersection()->normal);
        if (cosBeta < 0.0f)
            return glm::vec3(0.0f);

        // If we can't find any intersections (something gone wrong) or if the closest intersection
        // isn't on an emissive object, we are in shadow, return black.
        if (!findClosestIntersection(shadowRay) || !shadowRay->hitsEmissiveObject())
        {
            return glm::vec3(0.0f);
        }

        // Calculate the angle between the shadow ray (inverted) and the intersection on the emissive
        // object. If the angle is more than 90 degrees, we hit the light from behind, so return black.
        float cosAlpha = glm::dot(-1.f * shadowRayDirection, shadowRay->getIntersection()->normal);
        if (cosAlpha < 0.0f)
            return glm::vec3(0.0f);

        // Calculate the geometric term G(), how much contribution the shadow ray should give
        float d2 = shadowRay->lengthSquared();
        float geometricTerm = cosAlpha * cosBeta / d2;

        // Calculate brdf
        glm::vec3 brdf = originalRay->getValueOfBRDF(shadowRay);

        return geometricTerm * brdf;
    }

} // namespace rayTracer


