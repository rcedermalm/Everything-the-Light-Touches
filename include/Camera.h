
enum class ImageResolution {
    RESOLUTION_480p,
    RESOLUTION_720p,
    RESOLUTION_1080p
};

class Camera {

public:
    explicit Camera(ImageResolution imageResolution);

    void createImage();

private:
    int pixelHeight, pixelWidth;
};
