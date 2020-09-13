#include <SceneObject.h>
#include <Ray.h>
#include <cmath>
#include <MaterialProperties.h>

namespace rayTracer {

    const float EPSILON = 1e-6f;

    /**********************************/
    /***         SceneObject        ***/
    /**********************************/

    SceneObject::SceneObject(MaterialPtr inMaterial)
    : material(inMaterial)
    , surfaceArea(0.0f)
    , emittedRadiance(0.0f)
    {
    }

    ///----------------------------------------------

    void SceneObject::calculateRadiance()
    {
        if (auto emissiveMaterial = std::dynamic_pointer_cast<EmissiveMaterial>(material))
        {
            emittedRadiance = emissiveMaterial->getFlux() / (surfaceArea * glm::pi<float>());
        }
    }

    /**********************************/
    /***     SceneObject Sphere     ***/
    /**********************************/

    Sphere::Sphere(float inRadius, glm::vec3 inCenterPosition, MaterialPtr material)
    : SceneObject(material)
    , radius(inRadius)
    , centerPosition(inCenterPosition)
    {
        calculateArea();
        calculateRadiance();
    }

    ///----------------------------------------------

    bool Sphere::intersect(std::shared_ptr<Ray> currentRay)
    {
        glm::vec3 dirRayOriginToCenter = currentRay->getStartPoint() - centerPosition; //L
        float a = glm::dot(currentRay->getDirection(), currentRay->getDirection());
        float b = 2.f * glm::dot(currentRay->getDirection(), dirRayOriginToCenter);
        float c = glm::dot(dirRayOriginToCenter, dirRayOriginToCenter) - radius * radius;

        float d0, d1;
        if (!solveQuadratic(a, b, c, d0, d1)) return false;

        if (d0 > d1) std::swap(d0, d1);

        if (d0 < 0) {
            d0 = d1;
            if (d0 < 0) return false;
        }

        // We have an intersection
        if(currentRay->foundCloserRayIntersection(d0)){
            glm::vec3 intersectionPoint = currentRay->getStartPoint() + d0 * currentRay->getDirection();
            glm::vec3 intersectionNormal = glm::normalize(intersectionPoint - centerPosition);

            std::shared_ptr<Ray::Intersection> newIntersection = std::make_shared<Ray::Intersection>(
                    intersectionPoint, intersectionNormal, d0, material);

            currentRay->updateRayIntersection(newIntersection);
        }
        return true;
    }

    ///----------------------------------------------

    bool Sphere::solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
    {
        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0) return false;
        else if (glm::abs(discriminant) < EPSILON) x0 = x1 = -0.5f * b / a;
        else {
            float q = (b > 0) ?
                      -0.5f * (b + glm::sqrt(discriminant)) :
                      -0.5f * (b - glm::sqrt(discriminant));
            x0 = q / a;
            x1 = c / q;
        }
        if (x0 > x1) std::swap(x0, x1);

        return true;
    }

    ///----------------------------------------------

    void Sphere::calculateArea()
    {
        surfaceArea = 4.0f * glm::pi<float>() * float(glm::pow(radius, 2));
    }

    ///----------------------------------------------

    glm::vec3 Sphere::getRandomPointOnObject(
            std::shared_ptr<Ray> ray,
            std::mt19937* randGenerator,
            std::uniform_real_distribution<float>* randDistribution) const
    {
        // Grab a new random direction that is on the same hemisphere as the ray coming in.
        glm::vec3 newDir = ray->generateRandomReflectedRayDirection(randGenerator, randDistribution);
        return centerPosition + glm::normalize(newDir) * radius;
    }

    /**********************************/
    /***  SceneObject VertexObject  ***/
    /**********************************/

    VertexObject::VertexObject( std::vector<glm::vec3>& inVertices,
            std::vector<glm::ivec3>& inTriangleIndices, MaterialPtr material)
    : SceneObject(material)
    , vertices(inVertices)
    , triangleIndices(inTriangleIndices)
    {
        // Set triangle normals
        triangleNormals.reserve(triangleIndices.size());
        for (int triangle = 0; triangle < triangleIndices.size(); ++triangle)
            triangleNormals.push_back(calculateTriangleNormal(triangle));

        calculateArea();
        calculateRadiance();
    }

    ///----------------------------------------------

    glm::vec3 VertexObject::calculateTriangleNormal(int index)
    {
        glm::vec3 edge1 = vertices[triangleIndices[index][1]] - vertices[triangleIndices[index][0]];
        glm::vec3 edge2 = vertices[triangleIndices[index][2]] - vertices[triangleIndices[index][0]];
        return glm::normalize(glm::cross(edge1, edge2));
    }

    ///----------------------------------------------

    std::shared_ptr<VertexObject> VertexObject::createBox(glm::mat4x4 transform, MaterialPtr material)
    {
        // Set vertices
        std::vector<glm::vec3> boxVertices;
        boxVertices.reserve(8);
        boxVertices.emplace_back(-0.5f, 0.5f, 0.5f);
        boxVertices.emplace_back(0.5f, 0.5f, 0.5f);
        boxVertices.emplace_back(-0.5f, -0.5f, 0.5f);
        boxVertices.emplace_back(0.5f, -0.5f, 0.5f);
        boxVertices.emplace_back(-0.5f, 0.5f, -0.5f);
        boxVertices.emplace_back(0.5f, 0.5f, -0.5f);
        boxVertices.emplace_back(-0.5f, -0.5f, -0.5f);
        boxVertices.emplace_back(0.5f, -0.5f, -0.5f);

        for (auto& vertex : boxVertices){
            vertex = glm::vec3(transform * glm::vec4(vertex, 1.0f));
        }

        // Set triangle indices
        std::vector<glm::ivec3> boxTriangleIndices;
        boxTriangleIndices.reserve(12);
        boxTriangleIndices.emplace_back(0,2,1);
        boxTriangleIndices.emplace_back(1,2,3);
        boxTriangleIndices.emplace_back(1,3,5);
        boxTriangleIndices.emplace_back(3,7,5);
        boxTriangleIndices.emplace_back(4,5,7);
        boxTriangleIndices.emplace_back(4,7,6);
        boxTriangleIndices.emplace_back(0,4,6);
        boxTriangleIndices.emplace_back(0,6,2);
        boxTriangleIndices.emplace_back(0,1,4);
        boxTriangleIndices.emplace_back(1,5,4);
        boxTriangleIndices.emplace_back(2,6,3);
        boxTriangleIndices.emplace_back(3,6,7);

        return std::make_shared<VertexObject>(boxVertices, boxTriangleIndices, material);
    }

    ///----------------------------------------------

    std::shared_ptr<VertexObject> VertexObject::createPlane(
            glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, MaterialPtr material)
    {
        // Set vertices
        std::vector<glm::vec3> planeVertices;
        planeVertices.reserve(4);
        planeVertices.push_back(p0);
        planeVertices.push_back(p1);
        planeVertices.push_back(p2);
        planeVertices.push_back(p3);

        // Set triangle indices
        std::vector<glm::ivec3> planeTriangleIndices;
        planeTriangleIndices.reserve(2);
        planeTriangleIndices.emplace_back(0, 1, 2);
        planeTriangleIndices.emplace_back(2, 3, 0);

        return std::make_shared<VertexObject>(planeVertices, planeTriangleIndices, material);
    }

    ///----------------------------------------------

    bool VertexObject::intersectTriangle(std::shared_ptr<Ray> currentRay, int triangleIndex)
    {
        glm::vec3 edge1 = vertices[triangleIndices[triangleIndex][1]] - vertices[triangleIndices[triangleIndex][0]];
        glm::vec3 edge2 = vertices[triangleIndices[triangleIndex][2]] - vertices[triangleIndices[triangleIndex][0]];

        glm::vec3 T = currentRay->getStartPoint() - vertices[triangleIndices[triangleIndex][0]];
        glm::vec3 direction = currentRay->getDirection();
        glm::vec3 P = glm::cross(direction, edge2);
        glm::vec3 Q = glm::cross(T, edge1);

        float a = glm::dot(P, edge1);

        // Avoid division by 0
        if (std::fabs(a) < EPSILON) return false;

        float f = 1.0f / a;
        float u = glm::dot(P, T) * f;
        float v = glm::dot(Q, direction) * f;

        if (u + v > 1.0f || u < 0.0f || v < 0.0f) return false;

        float t = glm::dot(Q, edge2) * f;

        if(t > EPSILON && currentRay->foundCloserRayIntersection(t)){
            // We have an intersection
            glm::vec3 intersectionPoint = currentRay->getStartPoint() + t * currentRay->getDirection();
            glm::vec3 intersectionNormal = triangleNormals[triangleIndex];

            std::shared_ptr<Ray::Intersection> newIntersection = std::make_shared<Ray::Intersection>(
                    intersectionPoint, intersectionNormal, t, material);

            currentRay->updateRayIntersection(newIntersection);
            return true;
        }

        return false;
    }

    ///----------------------------------------------

    bool VertexObject::intersect(std::shared_ptr<Ray> currentRay)
    {
        bool intersection = false;
        for(int triangle = 0; triangle < triangleIndices.size(); ++triangle){
            if(intersectTriangle(currentRay, triangle) && !intersection)
                intersection = true;
        }

        return intersection;
    }

    ///----------------------------------------------

    void VertexObject::calculateArea()
    {
        float totalArea = 0;
        for(glm::ivec3 indices : triangleIndices){
            glm::vec3 edge1 = vertices[indices.x] - vertices[indices.y];
            glm::vec3 edge2 = vertices[indices.z] - vertices[indices.y];

            totalArea += (glm::length(glm::cross(edge1, edge2)) / 2.0f);
        }

        surfaceArea = totalArea;
    }

    ///----------------------------------------------

    glm::vec3 VertexObject::getRandomPointOnObject(
            std::shared_ptr<Ray> ray,
            std::mt19937* randGenerator,
            std::uniform_real_distribution<float>* randDistribution) const
    {
        // Get the negative direction so we have it point out of the object
        glm::vec3 direction = glm::normalize(-1.0f * ray->getDirection());

        // Retrieve a random triangle that has a normal that points in the right direction
        // (which will in a lot of the cases mean that it should be visible from the inDirection)
        int randomTriangleIndex = int((*randDistribution)(*randGenerator) * (triangleIndices.size() - 1));
//        auto angle = glm::pi<float>();
//        while (angle > glm::half_pi<float>())
//        {
//            randomTriangleIndex = int((*randDistribution)(*randGenerator) * triangleIndices.size());
//            angle = glm::acos(glm::dot(direction, triangleNormals[randomTriangleIndex]));
//        }

        // Get random point on triangle (uniform pdf(u,v) = 1/area)
        float u = 1, v = 1;
        while (u + v > 1.0f)
        {
            u = (*randDistribution)(*randGenerator);
            v = (*randDistribution)(*randGenerator);
        }

        glm::vec3 v0 = vertices[triangleIndices[randomTriangleIndex].x];
        glm::vec3 v1 = vertices[triangleIndices[randomTriangleIndex].y];
        glm::vec3 v2 = vertices[triangleIndices[randomTriangleIndex].z];

        return (1.0f - u - v) * v0 + u * v1 + v * v2;
    }

} // namespace rayTracer