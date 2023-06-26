#include "easeinsampler.hpp"

nitro::EaseInSampler::EaseInSampler() = default;

static inline float easeIn(float t) {
    return t * t;
}

float nitro::EaseInSampler::mix(float a, float b, float w) const {
    float t = easeIn(w);
    return (1.0f - t) * a + t * b;
}
