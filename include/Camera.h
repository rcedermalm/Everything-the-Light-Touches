#include <vector>
#include <fstream>
#include <glm.hpp>

#include "Scene.h"

enum class ImageResolution {
    RESOLUTION_480p,
    RESOLUTION_720p,
    RESOLUTION_1080p
};

class Camera {

public:
    explicit Camera(glm::vec3 eye,
                    glm::vec3 center,
                    glm::vec3 up,
                    float fov,
                    ImageResolution imageResolution);

    void setScene(std::shared_ptr<Scene> scene);

    void renderImage();

private:
    glm::vec3 eye, center, up;
    float fov;
    int pixelHeight, pixelWidth;
    glm::mat4 VP_inv;
    std::vector<std::vector<glm::vec3>> pixels;

    std::shared_ptr<Scene> sceneToRender;

    Ray castRay(int pixelX, int pixelY, float randomnessX, float randomnessY);
    glm::vec3 traceRay(Ray* ray);

    void generateImage();
};
