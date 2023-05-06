#pragma once

#include <QImage>

namespace nitro::operations {

    QVector<QImage> separateYCbCr(const QImage &img);

    QImage combineYCbCr(const QImage &yChannel, const QImage &cbChannel, const QImage &crChannel);


} // nitro::operations