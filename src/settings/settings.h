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
  // settings for the visualization of the mesh.
  ViewSettings view;

  bool uniformUpdateRequired = true;

} Settings;

#endif  // SETTINGS_H
