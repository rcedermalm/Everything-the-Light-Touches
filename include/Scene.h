#pragma once
#include <glm.hpp>
#include <memory>
#include <vector>

namespace rayTracer {

class MaterialProperties;
using MaterialPtr = std::shared_ptr<MaterialProperties>;
class SceneObject;
class Ray;

class Scene {
public:
    Scene() = default;

    static std::shared_ptr<Scene> createDefaultScene();

    bool findClosestIntersection(Ray* currentRay);

    /// ---------------------------------------------------------------------
    /// Functions to add objects to scene

    void addSphere(float radius, glm::vec3 centerPosition, MaterialPtr material);

    void addBox(glm::mat4x4 transform, MaterialPtr material );

    void addPlane(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, MaterialPtr material);

private:
    std::vector<std::shared_ptr<SceneObject>> sceneObjects;
};

} // namespace rayTracer