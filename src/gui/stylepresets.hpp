#pragma once

#include <QPalette>
#include <QColor>

class Settings;

namespace nitro {

using StylePreset = struct StylePreset {
    QPalette palette;
};

void applyStylePreset(const StylePreset &preset);

StylePreset getDarkModePalette();

StylePreset getLightModePalette();

} // namespace nitro
