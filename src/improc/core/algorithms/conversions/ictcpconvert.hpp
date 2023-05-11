#pragma once

#include <QImage>

namespace nitro::operations {

    QVector<QImage> separateICtCp(const QImage &img);

    QImage combineICtCp(const QImage &iChannel, const QImage &ctChannel, const QImage &cpChannel);


} // nitro::operations