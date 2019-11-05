#include "Ray.h"

Ray::Ray(glm::vec3 inStartPoint, glm::vec3 inDirection)
: startPoint(inStartPoint), direction(glm::normalize(inDirection)), rayIntersection(nullptr) {}

bool Ray::foundCloserRayIntersection(float newDistanceToOrigin) {
    return (!rayIntersection || rayIntersection->distanceToRayOrigin > newDistanceToOrigin );
}

void Ray::updateRayIntersection(std::shared_ptr<RayIntersection> newRayIntersection) {
    rayIntersection = newRayIntersection;
}
