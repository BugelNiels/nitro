#include "samplemethod.hpp"

std::shared_ptr<nitro::Resampler> getSampler(nitro::SampleMethod method) {
    return nitro::samplers[method];
}
