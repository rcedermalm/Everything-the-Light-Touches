#pragma once
#include <fstream>
#include <glm.hpp>
#include <memory>
#include <vector>

namespace rayTracer {

class Ray;
class Scene;

class Camera {
public:
    enum class ImageResolution {
        RESOLUTION_480p,
        RESOLUTION_720p,
        RESOLUTION_1080p
    };

    explicit Camera(glm::vec3 eye,
                    glm::vec3 center,
                    glm::vec3 up,
                    float fov,
                    ImageResolution imageResolution);

    /// Sets the scene that should be rendered.
    void setScene(std::shared_ptr<Scene> scene);

    /// Renders the image by sending out multiple rays per pixel into the scene.
    void renderImage();

private:
    glm::vec3 eye, center, up;
    float fov;
    int pixelHeight, pixelWidth;
    glm::mat4 VP_inv;
    std::vector<std::vector<glm::vec3>> pixels;

    std::shared_ptr<Scene> sceneToRender;

    static const int numSubSamples = 1;

    /// Creates a ray shooting out from pixel x and y. Possible to add some randomness [-0.5, 0.5] to it
    std::shared_ptr<Ray> castRay(int pixelX, int pixelY, float randomnessX, float randomnessY);

    /// Generates a .ppm image using the pixel values stored in 'pixels'
    void generateImage();
};

} // namespace rayTracer