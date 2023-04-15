#ifndef SAMPLESETTINGS_H
#define SAMPLESETTINGS_H

#include "resamplers/samplemethod.h"

typedef struct SampleSettings {
  int quantisizeLevel = 8;
  bool fastSdf = false;
  double distMult = 1.0f;
  bool autoSample = false;
  int sampleLevel = 255;
  SampleMethod sampleMethod = LINEAR;
  bool use3dDistanceField = false;

} SampleSettings;

#endif  // SAMPLESETTINGS_H
