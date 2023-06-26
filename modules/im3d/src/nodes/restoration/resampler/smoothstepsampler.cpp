#include "smoothstepsampler.hpp"

nitro::SmoothSampler::SmoothSampler() = default;

static inline float smoothStep(float edge0, float edge1, float x) {
    float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);

    return t * t * (3.0f - 2.0f * t);
}

float nitro::SmoothSampler::mix(float a, float b, float w) const {
    float t = smoothStep(0.0f, 1.0f, w);
    return (1.0f - t) * a + t * b;
}
