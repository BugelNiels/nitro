#ifndef IM_VIEW_SETTINGS_H
#define IM_VIEW_SETTINGS_H

enum DispImgType { ORIGINAL, QUANTISIZED, RESAMPLED };

typedef struct ImViewSettings {
  int activeImgIndex = 0;
  int compareImgIndex = 0;
} ImViewSettings;

#endif  // IM_VIEW_SETTINGS_H
