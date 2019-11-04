#include <glm.hpp>
#include <vector>
#include <memory>

#include "SceneObject.h"

class Scene {
public:
    Scene() = default;

    void addSphere(float radius, glm::vec3 centerPosition, MaterialProperties material);

    void addBox(float height, float width, float depth, glm::mat4x4 transform, MaterialProperties material );

    void addPlane(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, MaterialProperties material);

    static Scene createDefaultScene();

private:
    std::vector<SceneObject*> sceneObjects;
};
