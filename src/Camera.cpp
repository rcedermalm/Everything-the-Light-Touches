#include <Camera.h>
#include <Ray.h>
#include <Scene.h>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <random>

namespace rayTracer {

    namespace {

        void displayTimeTaken(int timeInMilliSeconds){
            int timeInMinutes = (timeInMilliSeconds/1000)/60;
            float restSeconds = (float(timeInMilliSeconds)/1000.0f) - float(timeInMinutes) * 60;

            std::cout << "Time taken: ";
            if(timeInMinutes > 0)
                std::cout << timeInMinutes << "min and ";
            std::cout << restSeconds << "s " << std::endl;
        }

    } // anonymous namespace

    ///----------------------------------------------

    Camera::Camera(glm::vec3 eye,
                   glm::vec3 center,
                   glm::vec3 up,
                   float fov,
                   ImageResolution imageResolution)
                   : eye (eye), center(center), up(up), fov(fov)
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

    void Camera::renderImage() {
        if (!sceneToRender) {
            std::cout << "There is no scene to render" << std::endl;
            return;
        }

        // For randomness in the ray generation
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(-0.5, 0.5);

        // For calculating time taken
        auto startTime = std::chrono::high_resolution_clock::now();

        // Calculate the pixel values by sending out rays into the scene
        int lastPercentageOutputted = -1;
        for (int i = 0; i < pixelHeight; i++)
        {
    #pragma omp parallel for
            for (int j = 0; j < pixelWidth; j++) {
                glm::vec3 finalColor = glm::vec3(0.0f);
                for (int subSample = 0; subSample < numSubSamples; ++subSample)
                {
                    std::shared_ptr<Ray> newRay = castRay(j, pixelHeight - i - 1, dis(gen), dis(gen));
                    finalColor += sceneToRender->traceRayThroughScene(newRay);
                }
                pixels[i][j] = finalColor / float(numSubSamples);
            }

            // Print out progress every x% done
            int printEveryXpercent = 2;
            int percentageDone = int(float(i+1) / float(pixelHeight) * 100);
            if (percentageDone % printEveryXpercent == 0 && percentageDone != lastPercentageOutputted)
            {
                std::cout << "[" << std::setw(3) << percentageDone << "%] ";
                lastPercentageOutputted = percentageDone;
                if (percentageDone == 0)
                    std::cout << "Starting off";
                else if (percentageDone == 50)
                    std::cout << "Halfway there!";
                else if (percentageDone == 100)
                    std::cout << "Done!";
                std::cout << std::endl;
            }
        }

        // Generate the image from the pixel values
        generateImage();

        // Calculate time taken
        auto endTime = std::chrono::high_resolution_clock::now();
        displayTimeTaken(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());
    }

    ///----------------------------------------------

    void Camera::generateImage() {
        FILE *f = fopen("../renderedImage.ppm", "wb");
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

    void Camera::setScene(std::shared_ptr<Scene> scene) {
        if (scene != nullptr)
            sceneToRender = scene;
    }

    ///----------------------------------------------

    std::shared_ptr<Ray> Camera::castRay(int pixelX, int pixelY, float randomnessX, float randomnessY) {
        glm::vec4 from4 = VP_inv *
                glm::vec4((((float)pixelX + randomnessX) / (float)pixelWidth - 0.5) * 2,
                        (((float)pixelY + randomnessY) / (float)pixelHeight - 0.5) * 2,
                        1, 1 );

        glm::vec4 to4 = VP_inv *
                glm::vec4((((float)pixelX + randomnessX) / (float)pixelWidth - 0.5) * 2,
                        (((float)pixelY + randomnessY) / (float)pixelHeight - 0.5) * 2,
                        -1, 1 );

        glm::vec3 from = glm::vec3(from4) * from4.w;
        glm::vec3 to = glm::vec3(to4) * to4.w;

        glm::vec3 direction = glm::normalize(to - from);
        return std::make_shared<Ray>(eye, direction);
    }

} // namespace rayTracer

