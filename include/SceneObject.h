#pragma once
#include <glm.hpp>
#include <memory>
#include <vector>
#include <random>

namespace rayTracer {

    class MaterialProperties;
    using MaterialPtr = std::shared_ptr<MaterialProperties>;
    class Ray;

    /**********************************/
    /***         SceneObject        ***/
    /**********************************/

    /// Abstract base class of the different possible objects
    /// that can be added to a scene
    class SceneObject {
    public:
        /// Checks if the given ray intersects the current object
        virtual bool intersect(std::shared_ptr<Ray> currentRay) = 0;

        /// Returns the area of the object
        float area() const { return surfaceArea; }

        float radiance() const { return emittedRadiance; }

        /// Returns a random point on the object where the surface normal
        /// is within 90 degrees of the negative rays direction (the naive
        /// way of checking if the point is visible from that direction)
        virtual glm::vec3 getRandomPointOnObject( std::shared_ptr<Ray> ray,
                std::mt19937* randGenerator,
                std::uniform_real_distribution<float>* randDistribution) const = 0;

    protected:
        explicit SceneObject(MaterialPtr inMaterial);

        void calculateRadiance();

        MaterialPtr material;
        float surfaceArea;
        float emittedRadiance;
    };

    /**********************************/
    /***     SceneObject Sphere     ***/
    /**********************************/

    class Sphere : public SceneObject
    {
    public:
        Sphere(float inRadius, glm::vec3 inCenterPosition, MaterialPtr material);

        /// Checks if the given ray intersects the current object
        bool intersect(std::shared_ptr<Ray> currentRay) override;

        /// Returns a random point on the object where the surface normal
        /// is within 90 degrees of the negative rays direction (the naive
        /// way of checking if the point is visible from that direction)
        glm::vec3 getRandomPointOnObject( std::shared_ptr<Ray> ray,
                                          std::mt19937* randGenerator,
                                          std::uniform_real_distribution<float>* randDistribution) const override;

    private:
        float radius;
        glm::vec3 centerPosition;

        bool static solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

        /// Calculates the area of the object
        void calculateArea();
    };

    /**********************************/
    /***  SceneObject VertexObject  ***/
    /**********************************/

    class VertexObject : public SceneObject
    {
    public:
        VertexObject(std::vector<glm::vec3> &inVertices, std::vector<glm::ivec3> &inTriangleIndices,
                     MaterialPtr material);

        /// Checks if the given ray intersects the current object
        bool intersect(std::shared_ptr<Ray> currentRay) override;

        /// Returns a random point on the object where the surface normal
        /// is within 90 degrees of the negative rays direction (the naive
        /// way of checking if the point is visible from that direction)
        glm::vec3 getRandomPointOnObject( std::shared_ptr<Ray> ray,
                                          std::mt19937* randGenerator,
                                          std::uniform_real_distribution<float>* randDistribution) const override;

        /// Factory functions to create specific vertex objects
        static std::shared_ptr<VertexObject> createBox(glm::mat4x4 transform, MaterialPtr material);
        static std::shared_ptr<VertexObject> createPlane(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
                                                         MaterialPtr material);

    private:
        std::vector<glm::vec3> vertices;
        std::vector<glm::ivec3> triangleIndices;
        std::vector<glm::vec3> triangleNormals;

        /// Calculates the normal of a triangle
        glm::vec3 calculateTriangleNormal(int index);

        /// Finding intersection point on a triangle using The Möller–Trumbore ray-triangle intersection algorithm
        bool intersectTriangle(std::shared_ptr<Ray> currentRay, int triangleIndex);

        /// Calculates the area of the object
        void calculateArea();
    };

} // namespace rayTracer