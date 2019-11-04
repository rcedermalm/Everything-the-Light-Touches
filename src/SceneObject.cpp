#include "SceneObject.h"

/**********************************/
/***         SceneObject        ***/
/**********************************/

SceneObject::SceneObject(MaterialProperties inMaterial)
: material(inMaterial) {}

/**********************************/
/***     SceneObject Sphere     ***/
/**********************************/

Sphere::Sphere(float inRadius, glm::vec3 inCenterPosition, MaterialProperties material)
: SceneObject(material), radius(inRadius), centerPosition(inCenterPosition) {}

/**********************************/
/***  SceneObject VertexObject  ***/
/**********************************/

VertexObject::VertexObject( std::vector<glm::vec3>& inVertices, std::vector<glm::ivec3>& inTriangleIndices,
                            MaterialProperties material) : SceneObject(material){
    vertices = inVertices;
    triangleIndices = inTriangleIndices;

    // Set triangle normals
    triangleNormals.reserve(triangleIndices.size());
    for (size_t triangle = 0; triangle < triangleIndices.size(); ++triangle)
        triangleNormals.push_back(calculateTriangleNormal(triangle));
}

///----------------------------------------------

glm::vec3 VertexObject::calculateTriangleNormal(int index) {
    glm::vec3 edge1 = vertices[triangleIndices[index][0]] - vertices[triangleIndices[index][1]];
    glm::vec3 edge2 = vertices[triangleIndices[index][2]] - vertices[triangleIndices[index][1]];
    return glm::normalize(glm::cross(edge1, edge2));
}

///----------------------------------------------

std::shared_ptr<VertexObject> VertexObject::createBox(
        float height, float width, float depth, glm::mat4x4 transform, MaterialProperties material)  {
    // Set vertices
    std::vector<glm::vec3> boxVertices;
    boxVertices.reserve(8);
    boxVertices.emplace_back(-1.0f * width/2.0f, height/2.0f, depth/2.0f);
    boxVertices.emplace_back(width/2.0f, height/2.0f, depth/2.0f);
    boxVertices.emplace_back(-1.0f * width/2.0f, -1.0f * height/2.0f, depth/2.0f);
    boxVertices.emplace_back(width/2.0f, -1.0f * height/2.0f, depth/2.0f);
    boxVertices.emplace_back(-1.0f * width/2.0f, height/2.0f, -depth/2.0f);
    boxVertices.emplace_back(width/2.0f, height/2.0f, -1.0f * depth/2.0f);
    boxVertices.emplace_back(-1.0f * width/2.0f, -1.0f * height/2.0f, -1.0f * depth/2.0f);
    boxVertices.emplace_back(width/2.0f, -1.0f * height/2.0f, -1.0f * depth/2.0f);

    for (auto vertex : boxVertices){
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
        glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, MaterialProperties material) {
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
