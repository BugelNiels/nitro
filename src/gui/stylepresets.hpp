#pragma once

#include <QPalette>
#include <QColor>

class Settings;

namespace nitro {

    typedef struct StylePreset {
        QPalette palette;
    } StylePreset;

    void applyStylePreset(const StylePreset &preset);

    StylePreset getDarkModePalette();

    StylePreset getLightModePalette();
} // nitro
