
#include <glm.hpp>
#include <vector>

#include "SceneObject.h"
#include "Camera.h"

class Scene {
public:
    Scene() = default;

    void addSphere(float radius, glm::vec3 centerPosition);

    void addBox(float height, float width, float depth, glm::mat4x4 transform );

    static Scene createDefaultScene();

private:
    std::vector<SceneObject*> sceneObjects;
};
