#pragma once

#include <QVector>
#include "src/core/cbdimage.hpp"

namespace nitro::operations {

    QVector<int> getHistogram(const nitro::CbdImage& img);

} // nitro
