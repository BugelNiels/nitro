#include "cbdimage.hpp"

#include <QDebug>

nitro::CbdImage::CbdImage() = default;

nitro::CbdImage::CbdImage(int width, int height, int dynRange)
        : numGreyLevels(dynRange), matrix(nitro::Matrix<int>(width, height)) {
}

nitro::CbdImage::CbdImage(const QImage &img) {
    int width = img.width();
    int height = img.height();
    matrix = nitro::Matrix<int>(width, height);
    numGreyLevels = 1 << img.depth();
    for (int y = 0; y < height; y++) {
        const uchar *inputRow = img.constScanLine(y);
        for (int x = 0; x < width; x++) {
            int val = inputRow[x];
            matrix.set(x, y, val);
        }
    }

}

const QImage &nitro::CbdImage::getDisplayImg() {
    if (displayCalculated) {
        return displayImg;
    }
    int width = matrix.width();
    int height = matrix.height();

    displayImg = QImage(width, height, QImage::Format_Grayscale8);

    for (int y = 0; y < height; y++) {
        uchar *outputRow = displayImg.scanLine(y);
        for (int x = 0; x < width; x++) {
            if (indexed) {
                outputRow[x] = vals[matrix.get(x, y)];
            } else {
                outputRow[x] = matrix.get(x, y);
            }
        }
    }
    displayCalculated = true;
    return displayImg;
}

// TODO: do this without copying
void nitro::CbdImage::setIndexed(QVector<int> t_vals) {
    indexed = true;
    this->vals = t_vals;
    numGreyLevels = t_vals.size();
}

int nitro::CbdImage::get(int x, int y) const {
    return indexed ? vals[matrix.get(x, y)] : matrix.get(x, y);
}
