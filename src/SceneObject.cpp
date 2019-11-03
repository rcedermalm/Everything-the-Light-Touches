#include "SceneObject.h"

/**********************************/
/***     SceneObject Sphere     ***/
/**********************************/

Sphere::Sphere(float inRadius, glm::vec3 inCenterPosition)
: SceneObject(), radius(inRadius), centerPosition(inCenterPosition) {}

/**********************************/
/***       SceneObject Box      ***/
/**********************************/

Box::Box(float height, float width, float depth, glm::mat4x4 transform)
: SceneObject() {
    // Set vertices
    vertices.reserve(8);
    vertices.emplace_back(-1.0f * width/2.0f, height/2.0f, depth/2.0f);
    vertices.emplace_back(width/2.0f, height/2.0f, depth/2.0f);
    vertices.emplace_back(-1.0f * width/2.0f, -1.0f * height/2.0f, depth/2.0f);
    vertices.emplace_back(width/2.0f, -1.0f * height/2.0f, depth/2.0f);
    vertices.emplace_back(-1.0f * width/2.0f, height/2.0f, -depth/2.0f);
    vertices.emplace_back(width/2.0f, height/2.0f, -1.0f * depth/2.0f);
    vertices.emplace_back(-1.0f * width/2.0f, -1.0f * height/2.0f, -1.0f * depth/2.0f);
    vertices.emplace_back(width/2.0f, -1.0f * height/2.0f, -1.0f * depth/2.0f);

    for (auto vertex : vertices){
        vertex = glm::vec3(transform * glm::vec4(vertex, 1.0f));
    }

    // Set triangle indices
    triangleIndices.reserve(12);
    triangleIndices.emplace_back(0,2,1);
    triangleIndices.emplace_back(1,2,3);
    triangleIndices.emplace_back(1,3,5);
    triangleIndices.emplace_back(3,7,5);
    triangleIndices.emplace_back(4,5,7);
    triangleIndices.emplace_back(4,7,6);
    triangleIndices.emplace_back(0,4,6);
    triangleIndices.emplace_back(0,6,2);
    triangleIndices.emplace_back(0,1,4);
    triangleIndices.emplace_back(1,5,4);
    triangleIndices.emplace_back(2,6,3);
    triangleIndices.emplace_back(3,6,7);

    // Set triangle normals
    triangleNormals.reserve(12);
    for (int triangle = 0; triangle < 12; ++triangle)
        triangleNormals.push_back(calculateTriangleNormal(triangle));
}

///----------------------------------------------

glm::vec3 Box::calculateTriangleNormal(int index) {
    glm::vec3 edge1 = vertices[triangleIndices[index][0]] - vertices[triangleIndices[index][1]];
    glm::vec3 edge2 = vertices[triangleIndices[index][2]] - vertices[triangleIndices[index][1]];
    return glm::normalize(glm::cross(edge1, edge2));
}


