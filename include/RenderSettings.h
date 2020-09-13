#pragma once

namespace rayTracer {

	struct RenderSettings
	{
		int numSubSamplesPerPixel;
		int numShadowRays;
		float russianRouletteCoefficient;
		int outputProgressEveryXPercent;

		RenderSettings()
			: numSubSamplesPerPixel(1)
			, numShadowRays(1)
			, russianRouletteCoefficient(0.9f)
			, outputProgressEveryXPercent(10)
		{ }
	};
}

