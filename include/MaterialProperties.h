#pragma once
#include <memory>

namespace rayTracer {

class MaterialProperties {

public:
    explicit MaterialProperties(glm::vec3 inColor) : color(inColor) {}

    glm::vec3 color;
};

using MaterialPtr = std::shared_ptr<MaterialProperties>;

} // namespace rayTracer