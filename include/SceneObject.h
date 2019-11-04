#include <glm.hpp>
#include <vector>
#include <MaterialProperties.h>
#include <memory>

/**********************************/
/***         SceneObject        ***/
/**********************************/

class SceneObject {

protected:
    explicit SceneObject(MaterialProperties inMaterial);

private:
    MaterialProperties material;
};

/**********************************/
/***     SceneObject Sphere     ***/
/**********************************/

class Sphere: public SceneObject {

public:
    Sphere(float inRadius, glm::vec3 inCenterPosition, MaterialProperties material);

private:
    float radius;
    glm::vec3 centerPosition;
};

/**********************************/
/***  SceneObject VertexObject  ***/
/**********************************/

class VertexObject: public SceneObject {
public:
    VertexObject(std::vector<glm::vec3>& inVertices, std::vector<glm::ivec3>& inTriangleIndices, MaterialProperties material);

    // Factory functions to create specific vertex objects
    static std::shared_ptr<VertexObject> createBox(float height, float width, float depth, glm::mat4x4 transform, MaterialProperties material);
    static std::shared_ptr<VertexObject> createPlane(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, MaterialProperties material);

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> triangleIndices;
    std::vector<glm::vec3> triangleNormals;

    glm::vec3 calculateTriangleNormal(int index);
};


