#include <Camera.h>
#include <Ray.h>
#include <iostream>

namespace rayTracer {

    Camera::Camera(glm::vec3 eye,
                   glm::vec3 center,
                   glm::vec3 up,
                   float fov,
                   ImageResolution imageResolution,
                   std::string name)
                   : name(name), eye (eye), center(center), up(up), fov(fov)
    {
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

        // View and perspective matrices are used in the unProject() function
        glm::mat4 V = glm::lookAt(eye, center, up);
        float aspect = float(pixelWidth) / (float)pixelHeight;
        glm::mat4 P = glm::perspective(fov, aspect, 0.1f, 100.0f);
        VP_inv = glm::inverse(V * P);
    }

    ///----------------------------------------------

    std::shared_ptr<Ray> Camera::createCameraRay(int pixelX, int pixelY, float randomnessX, float randomnessY)
    {
        glm::vec4 from4 = VP_inv *
            glm::vec4((((float)pixelX + randomnessX) / (float)pixelWidth - 0.5) * 2,
                (((float)pixelY + randomnessY) / (float)pixelHeight - 0.5) * 2,
                1, 1);

        glm::vec4 to4 = VP_inv *
            glm::vec4((((float)pixelX + randomnessX) / (float)pixelWidth - 0.5) * 2,
                (((float)pixelY + randomnessY) / (float)pixelHeight - 0.5) * 2,
                -1, 1);

        glm::vec3 from = glm::vec3(from4) * from4.w;
        glm::vec3 to = glm::vec3(to4) * to4.w;

        glm::vec3 direction = glm::normalize(to - from);
        return std::make_shared<Ray>(eye, direction);
    }

    ///----------------------------------------------

    void Camera::setPixelValue(int x, int y, glm::vec3 pixelValue)
    {
        if (x < 0 || x >= pixelHeight || y < 0 || y >= pixelWidth)
            return;

        pixels[x][y] = pixelValue;
    }

    ///----------------------------------------------

    void Camera::generateImage() {
        FILE* f;
        fopen_s(&f, "../renderedImage.ppm", "wb");
        
        if (!f)
        {
            std::cout << "Can't open file, closing down.." << std::endl;
            return;
        }

        fprintf(f, "P6\n%i %i 255\n", pixelWidth, pixelHeight);

        for (int i = 0; i < pixelHeight; i++) {
            for (int j = 0; j < pixelWidth; j++) {
                glm::vec3 pixel = pixels[i][j] * glm::vec3(255,255,255);
                fputc((int) (pixel.r), f);
                fputc((int) (pixel.g), f);
                fputc((int) (pixel.b), f);
            }
        }
        fclose(f);
    }

    ///----------------------------------------------

    int Camera::getPixelHeight() const
    {
        return pixelHeight;
    }

    ///----------------------------------------------

    int Camera::getPixelWidth() const
    {
        return pixelWidth;
    }

    ///----------------------------------------------

    std::string Camera::getName() const
    {
        return name;
    }

} // namespace rayTracer

