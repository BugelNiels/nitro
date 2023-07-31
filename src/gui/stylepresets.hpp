#pragma once

#include <QColor>
#include <QPalette>

namespace nitro {

/**
 * @brief Struct containing the information for the application style.
 */
using StylePreset = struct StylePreset { QPalette palette; };

/**
 * @brief Applies the provided style preset to the application.
 * @param preset The style preset to use.
 */
void applyStylePreset(const StylePreset &preset);

/**
 * @brief Constructs a default dark mode style present.
 * @return A default dark mode style present.
 */
StylePreset getDarkModePalette();

/**
 * @brief Constructs a default light mode style present.
 * @return A default light mode style present.
 */
StylePreset getLightModePalette();

} // namespace nitro
