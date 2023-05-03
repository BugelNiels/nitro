#pragma once

#include "cbdimage.hpp"

namespace nitro::operations {

    nitro::CbdImage addImage(const nitro::CbdImage &inputImg, int val);

    nitro::CbdImage subtractImage(const nitro::CbdImage &inputImg, int val);

    nitro::CbdImage multiplyImage(const nitro::CbdImage &inputImg, double val);

    nitro::CbdImage divideImage(const nitro::CbdImage &inputImg, double val);

} // nitro::operations