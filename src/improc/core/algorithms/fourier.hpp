#pragma once

#include <complex>
#include "modules/imCore/internal/matrix.hpp"
#include "modules/imCore/internal/cbdimage.hpp"

namespace nitro {

    nitro::Matrix<std::complex<double>> fft2D(const nitro::CbdImage &image);

    // inverse fast fourier transform
    nitro::CbdImage ifft2D(const nitro::Matrix<std::complex<double>> &image, int refWidth = -1, int refHeight = -1);

    // swaps quadrants so that DC component is centered
    void fft2Dshift(nitro::Matrix<std::complex<double>> &image);

    // inverse shift
    void ifft2Dshift(nitro::Matrix<std::complex<double>> &image);

} // nitro
