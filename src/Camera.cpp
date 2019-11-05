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

    pixels.resize(pixelHeight);
    for ( int i = 0 ; i < pixelHeight ; i++ )
        pixels[i].resize(pixelWidth);
}

///----------------------------------------------

void Camera::renderImage() {
    generateImage();
}

///----------------------------------------------

void Camera::generateImage() {
    FILE *f = fopen("renderedImage.ppm", "wb");
    fprintf(f, "P6\n%i %i 255\n", pixelWidth, pixelHeight);

    for (int i = 0; i < pixelHeight; i++) {
        for (int j = 0; j < pixelWidth; j++) {
            glm::vec3 pixel = pixels[i][j];
            fputc((int) (pixel.r), f);
            fputc((int) (pixel.g), f);
            fputc((int) (pixel.b), f);
        }
    }
    fclose(f);
}

