#include <iostream>
#include <Camera.h>

int main() {
    std::cout << "~ Everything the light touches ~" << std::endl;

    // Create camera
    Camera camera(glm::vec3(0, 0, 2.8), // Eye (position of camera)
                  glm::vec3(0, 0, 0), // Center (position to look at)
                  glm::vec3(0, 1, 0), // Up vector
                  M_PI / 3.5, // Field of view in radians
                  ImageResolution::RESOLUTION_720p);

    // Create scene and add it to the camera
    std::shared_ptr<Scene> scene = Scene::createDefaultScene();
    camera.setScene(scene);

    // Render scene through the camera
    camera.renderImage();

    return 0;
}