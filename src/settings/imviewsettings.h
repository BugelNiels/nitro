#ifndef IM_VIEW_SETTINGS_H
#define IM_VIEW_SETTINGS_H

enum DispImgType { ORIGINAL, QUANTISIZED, RESAMPLED };

typedef struct ImViewSettings {
  DispImgType activeImage = DispImgType::ORIGINAL;
  DispImgType compareImage = DispImgType::ORIGINAL;
} ImViewSettings;

#endif  // IM_VIEW_SETTINGS_H
