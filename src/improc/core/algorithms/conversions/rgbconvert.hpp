#pragma once

#include <QImage>

namespace nitro::operations {

    QImage combineRgb(const QImage& red, const QImage& green, const QImage& blue);
    QVector<QImage> separateRgb(const QImage &img);

} // nitro::operations
