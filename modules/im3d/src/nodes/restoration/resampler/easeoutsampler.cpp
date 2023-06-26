#include "easeoutsampler.hpp"

nitro::EaseOutSampler::EaseOutSampler() = default;

static inline float easeOut(float t) {
    return 1.0f - (1.0f - t) * (1.0f - t);
}

float nitro::EaseOutSampler::mix(float a, float b, float w) const {
    float t = easeOut(w);
    return (1.0f - t) * a + t * b;
}
