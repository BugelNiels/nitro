
#include "util.hpp"

QColor nitro::makeReadable(const QColor &color, bool lightMode) {
    // Convert to YIQ color space
    double y = 0.299 * color.redF() + 0.587 * color.greenF() + 0.114 * color.blueF();
    double i = 0.596 * color.redF() - 0.274 * color.greenF() - 0.322 * color.blueF();
    double q = 0.211 * color.redF() - 0.523 * color.greenF() + 0.312 * color.blueF();

    // Adjust Y value to make color more readable on dark background
    if (lightMode) {
        y = qMin(0.4, y);
    } else {
        y = qMax(0.5, y);
    }

    // Convert back to RGB color space
    double r = y + 0.956 * i + 0.621 * q;
    double g = y - 0.272 * i - 0.647 * q;
    double b = y - 1.105 * i + 1.702 * q;

    // Clamp values to valid range
    r = qBound(0.0, r, 1.0);
    g = qBound(0.0, g, 1.0);
    b = qBound(0.0, b, 1.0);

    // Return adjusted color
    return QColor::fromRgbF(r, g, b, color.alphaF());
}