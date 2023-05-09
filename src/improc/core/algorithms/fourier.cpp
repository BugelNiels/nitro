#include "fourier.hpp"
#include <QDebug>
#include <cmath>

#include <vector>

typedef std::complex<double> dComplex;
using namespace std::complex_literals;

// FFT function
static void fft(std::vector<std::complex<double>> &x) {
    int n = x.size();
    if (n <= 1) {
        return;
    }

    std::vector<std::complex<double>> even(n / 2), odd(n / 2);
    for (int i = 0; i < n / 2; ++i) {
        even[i] = x[2 * i];
        odd[i] = x[2 * i + 1];
    }

    fft(even);
    fft(odd);

    for (int k = 0; k < n / 2; ++k) {
        std::complex<double> t = std::polar(1.0, -2 * M_PI * k / n) * odd[k];
        x[k] = even[k] + t;
        x[k + n / 2] = even[k] - t;
    }
}

// IFFT function
static void ifft(std::vector<std::complex<double>> &x) {
    // take the conjugate of the input
    for (auto &element: x) {
        element = std::conj(element);
    }

    // apply FFT
    fft(x);

    // take the conjugate of the output and normalize
    for (auto &element: x) {
        element = std::conj(element) / static_cast<double>(x.size());
    }
}

// 2D FFT
static void fft2DInPlace(nitro::Matrix<std::complex<double>> &data) {
    int w = data.width();
    int h = data.height();

    // perform FFT on rows
    std::vector<std::complex<double>> row(w);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            row[x] = data.get(x, y);
        }
        fft(row);
        for (int x = 0; x < w; x++) {
            data.set(x, y, row[x]);
        }
    }

    // perform FFT on columns
    std::vector<std::complex<double>> col(h);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            col[y] = data.get(x, y);
        }
        fft(col);
        for (int y = 0; y < h; y++) {
            data.set(x, y, col[y]);
        }
    }
}

// 2D IFFT
static void ifft2DInPlace(nitro::Matrix<std::complex<double>> &data) {
    int w = data.width();
    int h = data.height();

    // perform IFFT on rows
    std::vector<std::complex<double>> row(w);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            row[x] = data.get(x, y);
        }
        ifft(row);
        for (int x = 0; x < w; x++) {
            data.set(x, y, row[x]);
        }
    }

    // perform IFFT on columns
    std::vector<std::complex<double>> col(h);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            col[y] = data.get(x, y);
        }
        ifft(col);
        for (int y = 0; y < h; y++) {
            data.set(x, y, col[y]);
        }
    }
}

// function to compute the magnitude of a complex number
static inline double magnitude(const std::complex<double>& z) {
    return std::sqrt(std::pow(z.real(), 2) + std::pow(z.imag(), 2));
}

// function to compute the logarithmic magnitude of a complex number
static inline double logMagnitude(const std::complex<double>& z) {
    return std::log10(1 + magnitude(z));
}

// 2D F

static inline int nextPower2(int n) {
    int i = 1 << int(std::log(n));
    while (i < n) {
        i <<= 1;
    }
    return i;
}

// 2D FFT implementation with normalization
nitro::Matrix<std::complex<double>> nitro::fft2D(const nitro::CbdImage &image) {
    int width = image.width();
    int height = image.height();

    // TODO: pad
    
    nitro::Matrix<std::complex<double>> complexImage(nextPower2(width), nextPower2(height));
    complexImage.fill(0);

// Copy data to complexImage
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            complexImage.set(x, y, std::complex<double>(image.get(x, y), 0));
        }
    }

// Compute FFT
    fft2DInPlace(complexImage);
    return complexImage;

}

// 2D IFFT implementation with normalization
nitro::CbdImage nitro::ifft2D(const nitro::Matrix<std::complex<double>> &image, int refWidth, int refHeight) {
    int width = image.width();
    int height = image.height();

    nitro::Matrix<std::complex<double>> complexImage(width, height);

// Copy data to complexImage
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            complexImage.set(x, y, image.get(x, y));
        }
    }

// Compute IFFT
    ifft2DInPlace(complexImage);

    int outWidth = refWidth > 0 ? refWidth : image.width();
    int outHeight = refHeight > 0 ? refHeight : image.height();
    nitro::CbdImage outputImage(outWidth, outHeight, 256);
// Compute magnitude and convert to logarithmic scale
    auto &dat = outputImage.data();
    for (int y = 0; y < outHeight; y++) {
        for (int x = 0; x < outWidth; x++) {
            dat.set(x, y, std::clamp(std::real(complexImage.get(x, y)), 0.0, 255.0));
        }
    }

    return outputImage;
}

static void swapComplexPixels(nitro::Matrix<dComplex> &image, int x1, int y1, int x2, int y2) {
    dComplex val1 = image.get(x1, y1);
    dComplex val2 = image.get(x2, y2);
    image.set(x1, y1, val2);
    image.set(x2, y2, val1);
}

// swaps quadrants so that DC component is centered
void nitro::fft2Dshift(nitro::Matrix<dComplex> &image) {
    int width = image.width();
    int height = image.height();
    int w2 = width / 2;
    int h2 = height / 2;
    for (int y = 0; y < h2; y++) {
        for (int x = 0; x < w2; x++) {
            // top left with bottom right
            swapComplexPixels(image, x, y, x + w2, y + h2);
            // top right with bottom left
            swapComplexPixels(image, x + w2, y, x, y + h2);
        }
    }
}

// inverse shift
void nitro::ifft2Dshift(nitro::Matrix<dComplex> &image) { fft2Dshift(image); }