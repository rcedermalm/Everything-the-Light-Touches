#pragma once
#include <Camera.h>
#include <RenderSettings.h>
#include <glm.hpp>
#include <map>
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

    /// Renders the current scene given the name/id of the camera to render from
    void render(const std::string cameraName, const RenderSettings& settings);

    /// ---------------------------------------------------------------------
    /// Functions to add objects to scene

    /// Adds a sphere with the specified settings to the scene
    void addSphere(float radius, glm::vec3 centerPosition, MaterialPtr material, bool emissive = false);

    /// Adds a box with the specified settings to the scene
    void addBox(glm::mat4x4 transform, MaterialPtr material, bool emissive = false );

    /// Adds a plane with the specified settings to the scene
    void addPlane(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, MaterialPtr material, bool emissive = false);

    /// Adds a camera to the scene
    void addCamera(std::shared_ptr<Camera> camera);

private:
    /// Trace the ray through the scene recursively
    glm::vec3 traceRay(std::shared_ptr<Ray> ray) const;

    /// Given a ray it will find the closest intersection point within
    /// the scene.
    bool findClosestIntersection(std::shared_ptr<Ray> currentRay) const;

    /// Calculates the direct lighting on a point in space
    glm::vec3 calculateDirectLighting(const std::shared_ptr<Ray> ray) const;

    /// Calculates the contribution from the given shadow ray on the intersection point of the original ray.
    glm::vec3 getShadowRayContribution(const std::shared_ptr<Ray> originalRay, std::shared_ptr<Ray> shadowRay) const;
    
private:
    std::vector<std::shared_ptr<SceneObject>> sceneObjects;
    std::vector<int> emissiveObjectIndices; // indices into scene objects

    std::map<std::string, std::shared_ptr<Camera>> sceneCameras;

    RenderSettings renderSettings;

    std::mt19937* gen;
    std::uniform_real_distribution<float>* dis;
};

} // namespace rayTracer