#pragma once

#include <QColor>
#include <QPalette>

namespace nitro {

/**
 * Struct containing the information for the application style.
 */
using StylePreset = struct StylePreset { QPalette palette; };

/**
 * Applies the provided style preset to the application.
 * @param preset The style preset to use.
 */
void applyStylePreset(const StylePreset &preset);

/**
 * Constructs a default dark mode style present.
 * @return A default dark mode style present.
 */
StylePreset getDarkModePalette();

/**
 * Constructs a default light mode style present.
 * @return A default light mode style present.
 */
StylePreset getLightModePalette();

} // namespace nitro
