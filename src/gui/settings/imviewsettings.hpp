#pragma once

namespace nitro {

enum DispImgType { ORIGINAL, QUANTISIZED, RESAMPLED };

typedef struct ImViewSettings {
  int activeImgIndex = 0;
  int compareImgIndex = 0;
} ImViewSettings;

}  // namespace nitro
