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
        QMatrix4x4 toWorldCoordsMatrix;
        QMatrix4x4 modelViewMatrix, projectionMatrix;
        QMatrix3x3 normalMatrix;
        ShaderType activeShader = ShaderType::SDF;
        bool uniformUpdateRequired = true;


        QVector3D boundingBoxDims = QVector3D(4, 4, 4);
        float alpha = 0.8f;


        float fov = 40.0f;
        float dispRatio = 16.0f / 9.0f;
        float rotAngle = 0.0f;
        float dragSensitivity = 5.0f;
        float distFromCamera = 5.0f;

    } Settings;
}  // namespace nitro
