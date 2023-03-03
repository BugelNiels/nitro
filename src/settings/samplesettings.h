#ifndef SAMPLESETTINGS_H
#define SAMPLESETTINGS_H

#include "resamplers/linearresampler.h"
#include "resamplers/resampler.h"

enum SampleMethod { LINEAR, CUBIC_HERMITE, FRITSCH_CARLSON };

typedef struct SampleSettings {
  int quantisizeLevel = 8;
  bool autoSample = false;
  int sampleLevel = 255;
  SampleMethod sampleMethod = LINEAR;
  Resampler* resampler = new LinearResampler();
  bool use3dDistanceField = false;

} SampleSettings;

#endif  // SAMPLESETTINGS_H
