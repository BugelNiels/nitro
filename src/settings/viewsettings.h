#ifndef VIEW_SETTINGS_H
#define VIEW_SETTINGS_H

#include <QVector3D>

#include "colorsettings.h"

/*
 * Contains all settings related to the view.
 */
typedef struct ViewSettings {
  float fov = 40.0f;
  float dispRatio = 16.0f / 9.0f;
  float rotAngle = 0.0f;
  float dragSensitivity = 5.0f;
  float distFromCamera = 5.0f;

  bool wireframeMode = false;
  bool showNormals = false;
  float alpha = 0.8f;
  QVector3D boundingBoxDims = QVector3D(4, 4, 4);

  ColorSettings cols;
} ViewSettings;

#endif  // VIEW_SETTINGS_H
