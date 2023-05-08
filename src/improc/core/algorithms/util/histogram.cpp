#include "histogram.hpp"

QVector<int> nitro::operations::getHistogram(const nitro::CbdImage &image) {
    QVector<int> hist(255); // Hardcode to 255 grey levels for now
    hist.fill(0);
    int width = image.width();
    int height = image.width();
    const auto &data = image.constData();
    if (image.isIndexed()) {
        auto cols = image.getColTransform();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                hist[cols[data.get(x, y)]]++;
            }
        }
    } else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                hist[data.get(x, y)]++;
            }
        }
    }
    return hist;
}