#include <Ray.h>
#include <MaterialProperties.h>
#include <gtx/rotate_vector.hpp>
#include <gtx/norm.hpp>

namespace rayTracer {

    namespace {
        void convertToSphericalCoords(const glm::vec3 direction, float &azimuth, float &inclination)
        {
            glm::vec3 normalizedDir = glm::normalize(direction);
            azimuth = glm::atan(normalizedDir.y / normalizedDir.x);
            inclination = glm::atan(glm::sqrt(glm::pow(normalizedDir.x, 2) + glm::pow(normalizedDir.y, 2)) / normalizedDir.z);
        }
    } // anonymous namespace

    Ray::Ray(glm::vec3 inStartPoint, glm::vec3 inDirection)
    : startPoint(inStartPoint), direction(glm::normalize(inDirection)), rayIntersection(nullptr)
    { }

    ///----------------------------------------------

    float Ray::lengthSquared() const
    {
        if (!rayIntersection)
            return -1;

        return glm::length2(rayIntersection->intersectionPoint - startPoint);
    }

    ///----------------------------------------------

    bool Ray::foundCloserRayIntersection(float newDistanceToOrigin)
    {
        return (!rayIntersection || rayIntersection->distanceToRayOrigin > newDistanceToOrigin );
    }

    ///----------------------------------------------

    void Ray::updateRayIntersection(std::shared_ptr<Intersection> newRayIntersection)
    {
        rayIntersection = newRayIntersection;
    }

    ///----------------------------------------------

    std::shared_ptr<Ray> Ray::generateReflectedRay(std::mt19937* randGenerator,
                                                   std::uniform_real_distribution<float>* randDistribution) const
    {
        if (!rayIntersection)
            return nullptr;

        // Offset to add to the reflected ray's start position to make sure we don't
        // start inside the intersected object
        glm::vec3 offset = 0.00001f * rayIntersection->normal;
        glm::vec3 reflectedStartPosition = rayIntersection->intersectionPoint + offset;

        glm::vec3 reflectedDir = glm::vec3(0.0f);

        if (std::dynamic_pointer_cast<PerfectMirrorMaterial>(rayIntersection->material))
        {
            // Create perfect reflected ray
            reflectedDir = glm::reflect(direction, rayIntersection->normal);
        }
        else
        {
            // Create a random reflected ray
            reflectedDir = generateRandomReflectedRayDirection(randGenerator, randDistribution);
        }

        return std::make_shared<Ray>(reflectedStartPosition, reflectedDir);
    }

    ///----------------------------------------------

    glm::vec3 Ray::generateRandomReflectedRayDirection(
            std::mt19937* randGenerator,
            std::uniform_real_distribution<float>* randDistribution) const
    {
        // Uniform distribution over a hemisphere
        float randAzimuth = (*randDistribution)(*randGenerator);
        float randInclination = (*randDistribution)(*randGenerator);

        float inclination = glm::acos(glm::sqrt(randInclination));
        float azimuth = (2.f * glm::pi<float>() * randAzimuth);

        // Calculate tangent vector
        glm::vec3 tangent = direction - glm::dot(direction, rayIntersection->normal) * rayIntersection->normal;

        // Create the randomized direction
        glm::vec3 reflectedDir = rayIntersection->normal;
        reflectedDir = glm::normalize(glm::rotate(
                reflectedDir,
                inclination,
                tangent));
        reflectedDir = glm::normalize(glm::rotate(
                reflectedDir,
                azimuth,
                rayIntersection->normal));

        return reflectedDir;
    }

    ///----------------------------------------------

    std::shared_ptr<Ray> Ray::generateRefractedRay() const
    {
        if(!rayIntersection)
            return nullptr;

        // TODO: Implement

        return nullptr;
    }

    ///----------------------------------------------

    std::shared_ptr<Ray> Ray::generateShadowRay(glm::vec3 pointOnLightSource) const
    {
        glm::vec3 offset = 0.00001f * rayIntersection->normal;
        glm::vec3 shadowRayStartPosition = rayIntersection->intersectionPoint + offset;
        glm::vec3 shadowRayDirection = pointOnLightSource - shadowRayStartPosition;

        return std::make_shared<Ray>(shadowRayStartPosition, shadowRayDirection);
    }

    ///----------------------------------------------

    glm::vec3 Ray::getValueOfBRDF(std::shared_ptr<Ray> reflectedRay) const
    {
        // Convert to local space
        glm::mat4 worldToLocal = rayIntersection->getWorldToLocalMatrix(direction);
        glm::vec3 wInLocal = glm::vec3(worldToLocal * glm::vec4(direction, 1.0f));
        glm::vec3 wOutLocal = glm::vec3(worldToLocal * glm::vec4(reflectedRay->getDirection(), 1.0f));

        // Convert to hemispherical coordinates
        float wInAzimuth, wInInclination, wOutAzimuth, wOutInclination;
        convertToSphericalCoords(wInLocal, wInAzimuth, wInInclination);
        convertToSphericalCoords(wOutLocal, wOutAzimuth, wOutInclination);

        return rayIntersection->material->getBRDF(wInAzimuth, wInInclination, wOutAzimuth, wOutInclination);
    }

    ///----------------------------------------------

    bool Ray::hitsDiffuseObject() const
    {
        return !(std::dynamic_pointer_cast<PerfectMirrorMaterial>(rayIntersection->material)
                 || std::dynamic_pointer_cast<EmissiveMaterial>(rayIntersection->material));

    }

    ///----------------------------------------------

    bool Ray::hitsEmissiveObject() const
    {
        if (std::dynamic_pointer_cast<EmissiveMaterial>(rayIntersection->material))
            return true;

        return false;
    }

} // namespace rayTracer