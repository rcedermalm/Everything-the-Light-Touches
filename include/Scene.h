#pragma once
#include <glm.hpp>
#include <memory>
#include <vector>
#include <random>

namespace rayTracer {

class MaterialProperties;
using MaterialPtr = std::shared_ptr<MaterialProperties>;
class SceneObject;
class Ray;

class Scene {
public:
    Scene();
    ~Scene();

    /// Creates and returns a Cornell Box scene
    static std::shared_ptr<Scene> createDefaultScene();

    /// Starts the tracing of the ray through the scene
    glm::vec3 traceRayThroughScene(std::shared_ptr<Ray> ray) const;

    /// ---------------------------------------------------------------------
    /// Functions to add objects to scene

    /// Adds a sphere with the specified settings to the scene
    void addSphere(float radius, glm::vec3 centerPosition, MaterialPtr material, bool emissive = false);

    /// Adds a box with the specified settings to the scene
    void addBox(glm::mat4x4 transform, MaterialPtr material, bool emissive = false );

    /// Adds a plane with the specified settings to the scene
    void addPlane(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, MaterialPtr material, bool emissive = false);

private:
    std::vector<std::shared_ptr<SceneObject>> sceneObjects;
    std::vector<int> emissiveObjectIndices; // indices into scene objects

    /// Trace the ray through the scene recursively
    glm::vec3 traceRay(std::shared_ptr<Ray> ray, const int depth) const;

    /// Given a ray it will find the closest intersection point within
    /// the scene.
    bool findClosestIntersection(std::shared_ptr<Ray> currentRay) const;

    /// Calculates the direct lighting on a point in space
    glm::vec3 calculateDirectLighting(const std::shared_ptr<Ray> ray) const;

    glm::vec3 getShadowRayContribution(const std::shared_ptr<Ray> originalRay, std::shared_ptr<Ray> shadowRay) const;

    std::mt19937* gen;
    std::uniform_real_distribution<float>* dis;
};

} // namespace rayTracer