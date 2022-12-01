#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMatrix4x4>

#include "shadertypes.h"
#include "viewsettings.h"

/**
 * Struct that contains all the settings of the program. Initialised with a
 * number of default values.
 */
typedef struct Settings {
  ViewSettings view;

  bool uniformUpdateRequired = true;
  ShaderType activeShader = ShaderType::VOXEL;

  QMatrix4x4 modelViewMatrix, projectionMatrix;
  QMatrix3x3 normalMatrix;
} Settings;

#endif  // SETTINGS_H
