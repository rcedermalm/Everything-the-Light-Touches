#pragma once
#include <memory>
#include <glm.hpp>

namespace rayTracer {

    /// Abstract material class, subclasses should be different BRDF models
    class MaterialProperties
    {
    public:
        /// The brdf takes an incoming light direction, wIn and outgoing direction, wOut,
        /// and returns the ratio of reflected radiance exiting along wOut to the irradiance
        /// incident on the surface from direction wIn. wIn and wOut is represented in
        /// hemispherical coordinates
        virtual glm::vec3 getBRDF( const float wInAzimuth, const float wInInclination,
                const float wOutAzimuth, const float wOutInclination) const = 0;

    protected:

        MaterialProperties();

        explicit MaterialProperties(glm::vec3 reflectionCoefficients);

        glm::vec3 rho; // constant reflection coefficient
        glm::vec3 rhoOverPi;
    };

    using MaterialPtr = std::shared_ptr<MaterialProperties>;

    class LambertianMaterial : public MaterialProperties
    {
    public:
        explicit LambertianMaterial(glm::vec3 reflectionCoefficients);

        glm::vec3 getBRDF( const float wInAzimuth, const float wInInclination,
                const float wOutAzimuth, const float wOutInclination) const override;
    };

    class OrenNayarMaterial : public MaterialProperties
    {
    public:
        OrenNayarMaterial(glm::vec3 reflectionCoefficients, float gaussianStandardDeviation );

        glm::vec3 getBRDF( const float wInAzimuth, const float wInInclination,
                           const float wOutAzimuth, const float wOutInclination) const override;

    private:
        float sigma;
    };

    class PerfectMirrorMaterial : public MaterialProperties
    {
    public:
        PerfectMirrorMaterial() = default;

        glm::vec3 getBRDF( const float wInAzimuth, const float wInInclination,
                           const float wOutAzimuth, const float wOutInclination) const override;
    };

    class EmissiveMaterial : public MaterialProperties
    {
    public:
        EmissiveMaterial(glm::vec3 reflectionCoefficients, float inFlux);

        glm::vec3 getBRDF( const float wInAzimuth, const float wInInclination,
                           const float wOutAzimuth, const float wOutInclination) const override;

        float getFlux() const { return flux; }

    private:
        float flux;
    };

} // namespace rayTracer