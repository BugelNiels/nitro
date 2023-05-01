#pragma once

#include <QImage>

namespace nitro {

    float mse(const QImage &a, const QImage &b);

    QVector<QImage> separateRGB(const QImage &img);

    QImage combineRGB(const QVector<QImage> &imgs);

    QImage combineYCbCr(const QVector<QImage> &imgs);

    QVector<QImage> separateYCbCr(const QImage &img);

    void rgb2YCbCr(int red, int green, int blue, uchar &y, uchar &cb, uchar &cr);

    void yCbCr2Rgb(int y, int cb, int cr, uchar &red, uchar &green, uchar &blue);

}  // namespace nitro
