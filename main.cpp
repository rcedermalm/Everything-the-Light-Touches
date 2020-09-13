#include <iostream>
#include <Camera.h>
#include <RenderSettings.h>
#include <Scene.h>

using rayTracer::Camera;
using rayTracer::RenderSettings;
using rayTracer::Scene;

int main() {
    std::cout << "~ Everything the light touches ~" << std::endl;

    // Create settings to use
    RenderSettings settings;
    settings.numSubSamplesPerPixel = 1;
    settings.numShadowRays = 3;
    settings.russianRouletteCoefficient = 0.9f;
    settings.outputProgressEveryXPercent = 2;

    // Create scene. The default is a cornell box.
    std::shared_ptr<Scene> scene = Scene::createDefaultScene();

    // Create camera
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(
        glm::vec3(0, 0, 2.8), // Eye (position of camera)
        glm::vec3(0, 0, 0), // Center (position to look at)
        glm::vec3(0, 1, 0), // Up vector
        glm::pi<float>() / 3.5f, // Field of view in radians
        Camera::ImageResolution::RESOLUTION_720p,
        "MainCamera");

    // Add camera to the scene
    scene->addCamera(camera);

    // Render the scene using the camera and the render settings defined
    scene->render("MainCamera", settings);

    return 0;
}