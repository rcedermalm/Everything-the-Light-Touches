#include "Scene.h"
#include <memory>

void Scene::addSphere(float radius, glm::vec3 centerPosition) {
    std::shared_ptr<Sphere> newSphere = std::make_shared<Sphere>(radius, centerPosition);
    sceneObjects.push_back(newSphere.get());
}

///----------------------------------------------

void Scene::addBox(float height, float width, float depth, glm::mat4x4 transform ) {
    std::shared_ptr<Box> newBox = std::make_shared<Box>(height, width, depth, transform);
    sceneObjects.push_back(newBox.get());
}

///----------------------------------------------

Scene Scene::createDefaultScene() {
    Scene defaultScene;
    defaultScene.addBox(1.0f, 1.0f, 1.0f, glm::mat4x4());
    defaultScene.addSphere(0.5f, glm::vec3());
    return defaultScene;
}


