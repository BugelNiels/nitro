#include "samplemethod.h"

std::shared_ptr<Resampler> getSampler(SampleMethod method) {
  return samplers[method];
}
