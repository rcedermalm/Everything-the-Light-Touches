#include <Ray.h>
#include <MaterialProperties.h>

namespace rayTracer {

Ray::Ray(glm::vec3 inStartPoint, glm::vec3 inDirection)
: startPoint(inStartPoint), direction(glm::normalize(inDirection)), rayIntersection(nullptr) {}

bool Ray::foundCloserRayIntersection(float newDistanceToOrigin) {
    return (!rayIntersection || rayIntersection->distanceToRayOrigin > newDistanceToOrigin );
}

void Ray::updateRayIntersection(std::shared_ptr<RayIntersection> newRayIntersection) {
    rayIntersection = newRayIntersection;
}

glm::vec3 Ray::getMaterialColorOfIntersectionPoint() {
    if(!rayIntersection)
        return glm::vec3(0,0,0);
    else
        return rayIntersection->materialAtIntersection->color;
}

} // namespace rayTracer