#include "samplemethod.hpp"

std::shared_ptr<nitro::Resampler> nitro::getSampler(nitro::SampleMethod method) {
    return nitro::samplers[method];
}
