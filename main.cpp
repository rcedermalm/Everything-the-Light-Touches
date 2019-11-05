#include <iostream>

#include <Camera.h>
#include <Scene.h>

int main() {
    std::cout << "~ Everything the light touches ~" << std::endl;

    Camera camera(ImageResolution::RESOLUTION_720p);
    camera.renderImage();
    Scene scene = Scene::createDefaultScene();

    return 0;
}