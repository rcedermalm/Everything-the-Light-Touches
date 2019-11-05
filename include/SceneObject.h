#include <glm.hpp>
#include <vector>
#include <memory>
#include <Ray.h>

/**********************************/
/***         SceneObject        ***/
/**********************************/

class SceneObject {
public:
    virtual bool intersect(Ray currentRay) = 0;

protected:
    explicit SceneObject(MaterialProperties inMaterial);

    MaterialProperties material;
};

/**********************************/
/***     SceneObject Sphere     ***/
/**********************************/

class Sphere: public SceneObject {

public:
    Sphere(float inRadius, glm::vec3 inCenterPosition, MaterialProperties material);

    bool intersect(Ray currentRay) override;

private:
    float radius;
    glm::vec3 centerPosition;

    bool static solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);
};

/**********************************/
/***  SceneObject VertexObject  ***/
/**********************************/

class VertexObject: public SceneObject {
public:
    VertexObject(std::vector<glm::vec3>& inVertices, std::vector<glm::ivec3>& inTriangleIndices, MaterialProperties material);

    bool intersect(Ray currentRay) override;

    // Factory functions to create specific vertex objects
    static std::shared_ptr<VertexObject> createBox(float height, float width, float depth, glm::mat4x4 transform, MaterialProperties material);
    static std::shared_ptr<VertexObject> createPlane(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, MaterialProperties material);

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> triangleIndices;
    std::vector<glm::vec3> triangleNormals;

    glm::vec3 calculateTriangleNormal(int index);
};
