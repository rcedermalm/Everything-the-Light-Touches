#include <glm.hpp>
#include <vector>

/**********************************/
/***         SceneObject        ***/
/**********************************/

class SceneObject {

protected:
    SceneObject() = default;
};

/**********************************/
/***     SceneObject Sphere     ***/
/**********************************/

class Sphere: public SceneObject {

public:
    Sphere(float inRadius, glm::vec3 inCenterPosition);

private:
    float radius;
    glm::vec3 centerPosition;
};

/**********************************/
/***       SceneObject Box      ***/
/**********************************/

class Box: public SceneObject {

public:
    Box(float height, float width, float depth, glm::mat4x4 transform);

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> triangleIndices;
    std::vector<glm::vec3> triangleNormals;

    glm::vec3 calculateTriangleNormal(int index);
};

