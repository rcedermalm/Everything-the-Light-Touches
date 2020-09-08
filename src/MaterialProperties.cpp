#include "MaterialProperties.h"

namespace rayTracer {

    MaterialProperties::MaterialProperties()
            : rho(glm::vec3(0.0f))
            , rhoOverPi(glm::vec3(0.0f))
    { }

    ///----------------------------------------------

    MaterialProperties::MaterialProperties(glm::vec3 reflectionCoefficients)
            : rho(reflectionCoefficients)
    {
        rhoOverPi = glm::one_over_pi<float>() * rho;
    }

    ///----------------------------------------------

    LambertianMaterial::LambertianMaterial(glm::vec3 reflectionCoefficients)
        : MaterialProperties(reflectionCoefficients)
    { }

    ///----------------------------------------------

    glm::vec3 LambertianMaterial::getBRDF(
            const float wInAzimuth, const float wInInclination,
            const float wOutAzimuth, const float wOutInclination) const
    {
        return rhoOverPi;
    }

    ///----------------------------------------------

    OrenNayarMaterial::OrenNayarMaterial(glm::vec3 reflectionCoefficients, float gaussianStandardDeviation )
        : MaterialProperties(reflectionCoefficients)
        , sigma(gaussianStandardDeviation)
    { }

    ///----------------------------------------------

    glm::vec3 OrenNayarMaterial::getBRDF(
            const float wInAzimuth, const float wInInclination,
            const float wOutAzimuth, const float wOutInclination) const
    {
        float sigmaPow2 = glm::pow(sigma, 2);

        float A = 1.0f - (sigmaPow2 / (2 * (sigmaPow2 + 0.33f)));
        float B = (0.45f * sigmaPow2) / (sigmaPow2 + 0.09f);
        float alpha = glm::max(wInAzimuth, wOutAzimuth);
        float beta = glm::min(wInAzimuth, wOutAzimuth);

        return rhoOverPi * (A + B * glm::max(0.0f, glm::cos(wInInclination - wOutInclination)) * glm::sin(alpha) * glm::sin(beta));
    }

    ///----------------------------------------------

    glm::vec3 PerfectMirrorMaterial::getBRDF(
            const float wInAzimuth, const float wInInclination,
            const float wOutAzimuth, const float wOutInclination) const
    {
        return glm::vec3(1.0f);
    }

    ///----------------------------------------------

    EmissiveMaterial::EmissiveMaterial(glm::vec3 reflectionCoefficients, float inFlux)
            : MaterialProperties(reflectionCoefficients)
            , flux(inFlux)
    { }

    ///----------------------------------------------

    glm::vec3 EmissiveMaterial::getBRDF(
            const float wInAzimuth, const float wInInclination,
            const float wOutAzimuth, const float wOutInclination) const
    {
        return rho;
    }

} // namespace rayTracer