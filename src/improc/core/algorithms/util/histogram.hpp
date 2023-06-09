#pragma once

#include <QVector>
#include "modules/imCore/internal/cbdimage.hpp"

namespace nitro::operations {

    QVector<int> getHistogram(const nitro::CbdImage& img);

} // nitro
