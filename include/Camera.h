#include <vector>
#include <fstream>
#include <glm.hpp>

enum class ImageResolution {
    RESOLUTION_480p,
    RESOLUTION_720p,
    RESOLUTION_1080p
};

class Camera {

public:
    explicit Camera(ImageResolution imageResolution);

    void renderImage();

private:
    int pixelHeight, pixelWidth;
    std::vector<std::vector<glm::vec3>> pixels;

    void generateImage();
};
