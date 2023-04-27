#include "quantisizer.hpp"

nitro::Quantisizer::Quantisizer() = default;

nitro::Quantisizer::~Quantisizer() = default;

nitro::CbdImage nitro::Quantisizer::quantisizeDither(const CbdImage &img,
                                                     int newNumGreyLevels) const {
    const auto &matrix = img.constData();
    int width = matrix.width();
    int height = matrix.height();

    Matrix<float> errMatrix(width, height);

    for (int i = 0; i < matrix.numElems(); i++) {
        errMatrix.set(i, float(matrix.get(i)));
    }

    CbdImage quantisized(width, height, newNumGreyLevels);
    auto &quantMatrix = quantisized.data();
    QVector<int> newCols = constructColorTable(img, newNumGreyLevels);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float oldPixel = errMatrix.get(x, y);
            int newPixel = findClosestValue(img, newCols, oldPixel);
            // newPixel is an index
            float newPixelF = float(newCols[newPixel]);
            float err = oldPixel - newPixelF;

            // Boundary conditions and errors
            if (x + 1 < width) {
                float errRight = errMatrix.get(x + 1, y) + err * 7 / 16.0f;
                if (y + 1 < height) {
                    float errBottomRight = errMatrix.get(x + 1, y + 1) + err / 16.0f;
                    errMatrix.set(x + 1, y + 1, errBottomRight);
                }
                errMatrix.set(x + 1, y, errRight);
            }
            if (y + 1 < height) {
                if (x - 1 >= 0) {
                    float errBottomLeft = errMatrix.get(x - 1, y + 1) + err * 3 / 16.0f;
                    errMatrix.set(x - 1, y + 1, errBottomLeft);
                }
                float errBottom = errMatrix.get(x, y + 1) + err * 5 / 16.0f;

                errMatrix.set(x, y + 1, errBottom);
            }

            quantMatrix.set(x, y, newPixel);
        }
    }

    quantisized.setIndexed(newCols);
    return quantisized;
}

nitro::CbdImage nitro::Quantisizer::quantisize(const CbdImage &img, int newNumGreyLevels,
                                               bool dither) const {
    if (dither) {
        return quantisizeDither(img, newNumGreyLevels);
    }
    const auto &matrix = img.constData();
    int width = matrix.width();
    int height = matrix.height();

    CbdImage quantisized(width, height, newNumGreyLevels);
    auto &quantMatrix = quantisized.data();
    QVector<int> newCols = constructColorTable(img, newNumGreyLevels);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int oldPixel = matrix.get(x, y);
            int newPixel = findClosestValue(img, newCols, oldPixel);
            quantMatrix.set(x, y, newPixel);
        }
    }

    quantisized.setIndexed(newCols);
    return quantisized;
}
