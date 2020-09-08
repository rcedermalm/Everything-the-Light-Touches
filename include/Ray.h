#pragma once
#include <glm.hpp>
#include <memory>
#include <random>

namespace rayTracer {

    class MaterialProperties;
    using MaterialPtr = std::shared_ptr<MaterialProperties>;

    class Ray
    {
    public:
        struct Intersection
        {
            Intersection(glm::vec3 point, glm::vec3 inNormal, float distance, MaterialPtr inMaterial)
                    : distanceToRayOrigin(distance), intersectionPoint(point), normal(inNormal), material(inMaterial)
            { }

            glm::mat4 getWorldToLocalMatrix(glm::vec3 rayDirection) const
            {
                glm::vec3 axisX = rayDirection - glm::dot(rayDirection, normal) * normal;
                glm::vec3 axisY = glm::cross(-1.f * axisX, normal);
                return glm::inverse(glm::mat4( axisX.x, axisX.y, axisX.z, 0.0,
                        axisY.x, axisY.y, axisY.z, 0.0,
                        normal.x, normal.y, normal.z, 0.0,
                        0.0, 0.0, 0.0, 1.0));
            }

            float distanceToRayOrigin;
            glm::vec3 intersectionPoint;
            glm::vec3 normal;
            MaterialPtr material;
        };

        Ray(glm::vec3 inStartPoint, glm::vec3 inDirection);

        /// Get functions
        glm::vec3 getStartPoint() { return startPoint; }
        glm::vec3 getDirection() { return direction; }
        const std::shared_ptr<Intersection> getIntersection() { return rayIntersection; }

        float lengthSquared() const;

        /// Returns true if the given new distance is shorter than what is stored
        bool foundCloserRayIntersection(float newDistanceToOrigin);

        /// Updates the intersection to the given intersection
        void updateRayIntersection(std::shared_ptr<Intersection> newRayIntersection);

        /// Generate new rays from the current one which will reflect/refract
        /// at the point of the current ray's intersection point.
        std::shared_ptr<Ray> generateReflectedRay(
                std::mt19937* randGenerator,
                std::uniform_real_distribution<float>* randDistribution) const;
        glm::vec3 generateRandomReflectedRayDirection(
                std::mt19937* randGenerator,
                std::uniform_real_distribution<float>* randDistribution) const;
        std::shared_ptr<Ray> generateRefractedRay() const;
        std::shared_ptr<Ray> generateShadowRay(glm::vec3 pointOnLightSource) const;

        /// Returns the value of the BRDF between the current ray and the reflected ray
        glm::vec3 getValueOfBRDF(std::shared_ptr<Ray> reflectedRay) const;

        /// Returns true if the ray has intersected with a diffuse object
        bool hitsDiffuseObject() const;

        /// Returns true if the ray has intersected with an emissive object
        bool hitsEmissiveObject() const;

    private:
        glm::vec3 startPoint;
        glm::vec3 direction;

        std::shared_ptr<Intersection> rayIntersection;
    };

} // namespace rayTracer
