#pragma once

#include <QMatrix4x4>

#include "shadertypes.hpp"
#include "viewsettings.hpp"

namespace nitro {

/**
 * Struct that contains all the settings of the program. Initialised with a
 * number of default values.
 */
typedef struct Settings {
  // settings for the visualization of the mesh.
  ViewSettings view;

  bool uniformUpdateRequired = true;

} Settings;
}  // namespace nitro
