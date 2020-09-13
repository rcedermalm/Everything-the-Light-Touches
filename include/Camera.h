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
                    ImageResolution imageResolution,
                    std::string name);

    /// Creates a ray shooting out from pixel x and y. Possible to add some randomness [-0.5, 0.5] to it
    std::shared_ptr<Ray> createCameraRay(int pixelX, int pixelY, float randomnessX, float randomnessY);

    // Sets the pixel value at pixel [x, y] to the given value.
    void setPixelValue(int x, int y, glm::vec3 pixelValue);

    /// Generates a .ppm image using the pixel values stored in 'pixels'
    void generateImage();

    /// Get functions for pixel height, pixel width and name
    int getPixelHeight() const;
    int getPixelWidth() const;
    std::string getName() const;

private:
    std::string name;

    glm::vec3 eye, center, up;
    float fov;
    int pixelHeight, pixelWidth;
    glm::mat4 VP_inv;

    std::vector<std::vector<glm::vec3>> pixels;
};

} // namespace rayTracer