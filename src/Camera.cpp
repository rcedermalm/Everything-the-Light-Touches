#include "Camera.h"

Camera::Camera(ImageResolution imageResolution) {
    switch(imageResolution){
        case ImageResolution::RESOLUTION_480p:
            pixelHeight = 480;
            pixelWidth = 640;
            break;
        case ImageResolution::RESOLUTION_720p:
            pixelHeight = 720;
            pixelWidth = 1280;
            break;
        case ImageResolution::RESOLUTION_1080p:
            pixelHeight = 1080;
            pixelWidth = 1920;
            break;
        default:
            pixelHeight = 100;
            pixelWidth = 100;
            break;
    }
}

///----------------------------------------------

void Camera::createImage() {

}

