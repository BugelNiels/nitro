#pragma once

#include "quantisizers/quantisizemethod.hpp"
#include "resamplers/samplemethod.hpp"

namespace nitro {

typedef struct SampleSettings {
  int quantisizeLevel = 3;
  double distMult = 1.0f;
  bool autoSample = false;
  int sampleLevel = 256;
  bool dither = false;
  SampleMethod sampleMethod = LINEAR;
  QuantisizeMethod quantMethod = DEFAULT;
  bool use3dDistanceField = false;

} SampleSettings;
}  // namespace nitro
