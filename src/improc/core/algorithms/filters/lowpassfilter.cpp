#include <iostream>
#include "lowpassfilter.hpp"

#include <QDebug>
#include "core/algorithms/fourier.hpp"

static nitro::Matrix<std::complex<double>> constructButterworthLowPass(int width, int height, float cutoff, int order, int refWidth, int refHeight) {
    nitro::Matrix<std::complex<double>> matrix(refWidth, refHeight);
    matrix.fill(0);


    int center_x = width / 2;
    int center_y = height / 2;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double distance = std::sqrt((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y));

            // Calculate Butterworth filter
            double blp = 1.0 / (1.0 + std::pow(distance / cutoff, 2 * order));
            matrix.set(x, y, blp);

        }
    }
    return matrix;
}


nitro::CbdImage nitro::operations::lowPassFilter(const nitro::CbdImage &image, float cutoff, int order) {
    int width = image.width();
    int height = image.height();
    auto ft = fft2D(image);

    fft2Dshift(ft);

    auto filter = constructButterworthLowPass(ft.width(), ft.height(), cutoff, order, ft.width(), ft.height());
    for (int y = 0; y < ft.height(); y++) {
        for (int x = 0; x < ft.width(); x++) {
            auto val = ft.get(x, y) * filter.get(x, y);
            ft.set(x, y, val);
        }
    }
    ifft2Dshift(ft);
    return ifft2D(ft, width, height);
}