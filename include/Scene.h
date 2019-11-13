#include <glm.hpp>
#include <vector>
#include <memory>

#include "SceneObject.h"

class Scene {
public:
    Scene() = default;

    static std::shared_ptr<Scene> createDefaultScene();

    bool findClosestIntersection(Ray* currentRay);

    /// ---------------------------------------------------------------------
    /// Functions to add objects to scene

    void addSphere(float radius, glm::vec3 centerPosition, MaterialProperties material);

    void addBox(glm::mat4x4 transform, MaterialProperties material );

    void addPlane(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, MaterialProperties material);

private:
    std::vector<std::shared_ptr<SceneObject>> sceneObjects;
};
