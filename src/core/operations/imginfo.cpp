#include "imginfo.hpp"

float nitro::operations::meanBrightness(const nitro::CbdImage &image) {
    int width = image.width();
    int height = image.width();
    int sum = 0;
    const auto &data = image.constData();
    if (image.isIndexed()) {
        auto cols = image.getColTransform();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                sum += cols[data.get(x, y)];
            }
        }
    } else {

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                sum += data.get(x, y);
            }
        }
    }
    return float(sum) / float(width * height);
}

// Does not handle color images!
float nitro::operations::meanBrightness(const QImage &image) {
    int width = image.width();
    int height = image.width();
    int sum = 0;
    for (int y = 0; y < height; y++) {
        const uchar *row = image.constScanLine(y);
        for (int x = 0; x < width; x++) {
            sum += row[x];
        }
    }
    return float(sum) / float(width * height);
}