#pragma once

#include <QImage>

namespace nitro::operations {

    QImage flipCompare(const QImage& imageA, const QImage& imageB, float& mse);

} // nitro::operations
