#include <Scene.h>
#include <SceneObject.h>
#include <MaterialProperties.h>
#include <Ray.h>

namespace rayTracer {

void Scene::addSphere(float radius, glm::vec3 centerPosition, MaterialPtr material) {
    std::shared_ptr<Sphere> newSphere = std::make_shared<Sphere>(radius, centerPosition, material);
    sceneObjects.push_back(newSphere);
}

///----------------------------------------------

void Scene::addBox(glm::mat4x4 transform, MaterialPtr material ) {
    std::shared_ptr<VertexObject> newBox = VertexObject::createBox(transform, material);
    sceneObjects.push_back(newBox);
}

///----------------------------------------------

void Scene::addPlane(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, MaterialPtr material) {
    std::shared_ptr<VertexObject> newPlane = VertexObject::createPlane(p0, p1, p2, p3, material);
    sceneObjects.push_back(newPlane);
}

///----------------------------------------------

std::shared_ptr<Scene> Scene::createDefaultScene() {
    std::shared_ptr<Scene> defaultScene = std::make_shared<Scene>();

    // Create cornell box
    MaterialPtr diffuseRed = std::make_shared<MaterialProperties>(glm::vec3(1.f, 0.f,0.f));
    MaterialPtr diffuseWhite = std::make_shared<MaterialProperties>(glm::vec3(1.f, 1.f,1.f));
    MaterialPtr diffuseGreen = std::make_shared<MaterialProperties>(glm::vec3(0.f, 1.f,0.f));

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
    MaterialPtr diffuseMagenta = std::make_shared<MaterialProperties>(glm::vec3(1.f, 0.f,1.f));
    MaterialPtr diffuseCyan = std::make_shared<MaterialProperties>(glm::vec3(0.f, 1.f,1.f));

    glm::mat4x4 boxTransform = glm::mat4x4(1.0f);
    boxTransform = glm::translate(boxTransform, glm::vec3(-0.8, -0.3, -0.3));
    boxTransform = glm::rotate(boxTransform, (float)M_PI / 3, glm::vec3(0, 1, 0));
    boxTransform = glm::scale(boxTransform, glm::vec3(0.8,1.3,0.8));
    defaultScene->addBox(boxTransform, diffuseMagenta);
    defaultScene->addSphere(0.3f, glm::vec3(0.4f, -0.5f, 0.0f), diffuseCyan);

    return defaultScene;
}

bool Scene::findClosestIntersection(Ray* currentRay) {
    bool intersection = false;
    for(auto& sceneObject : sceneObjects){
        if(sceneObject->intersect(currentRay) && !intersection)
            intersection = true;
    }
    return intersection;
}

} // namespace rayTracer


