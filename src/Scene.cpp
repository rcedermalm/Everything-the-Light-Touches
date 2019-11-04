#include "Scene.h"

void Scene::addSphere(float radius, glm::vec3 centerPosition, MaterialProperties material) {
    std::shared_ptr<Sphere> newSphere = std::make_shared<Sphere>(radius, centerPosition, material);
    sceneObjects.push_back(newSphere.get());
}

///----------------------------------------------

void Scene::addBox(float height, float width, float depth, glm::mat4x4 transform, MaterialProperties material ) {
    std::shared_ptr<VertexObject> newBox = VertexObject::createBox(height, width, depth, transform, material);
    sceneObjects.push_back(newBox.get());
}

///----------------------------------------------

void Scene::addPlane(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, MaterialProperties material) {
    std::shared_ptr<VertexObject> newPlane = VertexObject::createPlane(p0, p1, p2, p3, material);
    sceneObjects.push_back(newPlane.get());
}

///----------------------------------------------

Scene Scene::createDefaultScene() {
    Scene defaultScene;

    // Create cornell box
    MaterialProperties diffuseRed(glm::vec3(1.f, 0.f,0.f));
    MaterialProperties diffuseWhite(glm::vec3(1.f, 1.f,1.f));
    MaterialProperties diffuseGreen(glm::vec3(0.f, 1.f,0.f));

    glm::vec3 p0 = glm::vec3(-1.5f, -1.f, -1.f);
    glm::vec3 p1 = glm::vec3(1.5f, -1.f, -1.f);
    glm::vec3 p2 = glm::vec3(1.5f, 1.f, -1.f);
    glm::vec3 p3 = glm::vec3(-1.5f, 1.f, -1.f);
    glm::vec3 p4 = glm::vec3(-1.5f, -1.f, 4.f);
    glm::vec3 p5 = glm::vec3(1.5f, -1.f, 4.f);
    glm::vec3 p6 = glm::vec3(1.5f, 1.f, 4.f);
    glm::vec3 p7 = glm::vec3(-1.5f, 1.f, 4.f);

    defaultScene.addPlane(p0, p1, p2, p3, diffuseWhite); // Back wall
    defaultScene.addPlane(p4, p7, p6, p5, diffuseWhite); // Front wall
    defaultScene.addPlane(p0, p3, p7, p4, diffuseRed); // Left wall
    defaultScene.addPlane(p1, p5, p6, p2, diffuseGreen); // Right wall
    defaultScene.addPlane(p2, p6, p7, p3, diffuseWhite); // Roof
    defaultScene.addPlane(p0, p4, p5, p1, diffuseWhite); // Floor

    // Add objects inside cornell box
    MaterialProperties diffuseMagenta(glm::vec3(1.f, 0.f,1.f));
    MaterialProperties diffuseCyan(glm::vec3(0.f, 1.f,1.f));

    defaultScene.addBox(0.3f, 0.3f, 0.3f,
            glm::translate(glm::mat4(), glm::vec3(0, -0.4, 0.1)), diffuseMagenta);
    defaultScene.addSphere(0.3f, glm::vec3(0.9f, -0.7f, 0.9f), diffuseCyan);

    return defaultScene;
}


