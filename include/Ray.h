#pragma once
#include <glm.hpp>
#include <memory>

namespace rayTracer {

class MaterialProperties;
using MaterialPtr = std::shared_ptr<MaterialProperties>;

class RayIntersection {
public:
    RayIntersection(glm::vec3 point, glm::vec3 normal, float distance, MaterialPtr material)
    : distanceToRayOrigin(distance), intersectionPoint(point), normalAtIntersection(normal), materialAtIntersection(material) {}

    float distanceToRayOrigin;
    glm::vec3 intersectionPoint;
    glm::vec3 normalAtIntersection;
    MaterialPtr materialAtIntersection;
};

class Ray {
public:
    Ray(glm::vec3 inStartPoint, glm::vec3 inDirection);

    glm::vec3 getStartPoint() { return startPoint; }
    glm::vec3 getDirection() { return direction; }

    glm::vec3 getMaterialColorOfIntersectionPoint();

    bool foundCloserRayIntersection(float newDistanceToOrigin);

    void updateRayIntersection(std::shared_ptr<RayIntersection> newRayIntersection);

private:
    glm::vec3 startPoint;
    glm::vec3 direction;

    std::shared_ptr<RayIntersection> rayIntersection;
};

} // namespace rayTracer
