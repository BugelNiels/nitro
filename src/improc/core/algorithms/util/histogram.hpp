#pragma once

#include <QVector>
#include "core/cbdimage.hpp"

namespace nitro::operations {

    QVector<int> getHistogram(const nitro::CbdImage& img);

} // nitro
