#pragma once

#include <QPalette>
#include <QColor>

namespace nitro {

using StylePreset = struct StylePreset {
    QPalette palette;
};

void applyStylePreset(const StylePreset &preset);

StylePreset getDarkModePalette();

StylePreset getLightModePalette();

} // namespace nitro
