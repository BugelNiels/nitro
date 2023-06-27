#pragma once

#include <QMatrix4x4>

#include "shadertypes.hpp"

namespace nitro {

/**
 * Struct that contains all the settings of the program. Initialised with a
 * number of default values.
 */
    typedef struct Settings {

        // save this matrix here to prevent recalculating this every time the mouse is
        // pressed.
        QMatrix4x4 cameraMatrix;
        QMatrix4x4 projectionMatrix;
        ShaderType activeShader = ShaderType::SURFACE;
        bool uniformUpdateRequired = true;

        float alpha = 0.8f;

        float fov = 40.0f;
        float dispRatio = 16.0f / 9.0f;
        float dragSensitivity = 100.0f;
        float distFromCamera = 256.0f;

        // TODO: Initialize these from elsewhere
        bool imageColors = true;
        bool orthographic = false;

        bool minecraft = false;
    } Settings;
}  // namespace nitro
