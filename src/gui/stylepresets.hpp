#pragma once

#include <QColor>
#include <QPalette>

namespace nitro {

using StylePreset = struct StylePreset { QPalette palette; };

void applyStylePreset(const StylePreset &preset);

StylePreset getDarkModePalette();

StylePreset getLightModePalette();

} // namespace nitro
