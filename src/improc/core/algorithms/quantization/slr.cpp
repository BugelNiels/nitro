#include "slr.hpp"

#include "core/algorithms/util/histogram.hpp"
#include <QtAlgorithms>
#include <QDebug>

typedef struct IndexedVal {
    int val;
    int idx;
} IndexedVal;

static inline int findClosestValue(const QVector<int> &newCols, int val) {
    int idx = 0;
    int curDistance = std::abs(val - newCols[0]);
    int k = newCols.size();
    for (int i = 1; i < k; i++) {
        int dist = std::abs(val - newCols[i]);
        if (dist < curDistance) {
            curDistance = dist;
            idx = i;
        }
    }
    return idx;
}

nitro::CbdImage nitro::operations::smartLayerRemoval(const nitro::CbdImage &img, int k) {

    auto hist = getHistogram(img);
    QVector<IndexedVal> histIndexed(hist.size());
    for (int i = 0; i < hist.size(); i++) {
        histIndexed[i] = {hist[i], i};
    }

    // TODO Local maxima
    std::sort(histIndexed.begin(), histIndexed.end(),
          [](const IndexedVal &a, const IndexedVal &b) { return a.val > b.val; });

    QVector<int> newCols(k);
    for (int i = 0; i < k; i++) {
        newCols[i] = histIndexed[i].idx;
    }
    std::sort(newCols.begin(), newCols.end(), [](int a, int b) { return a > b; });
    for (int i = 0; i < k; i++) {
        qDebug() << newCols[i];
    }
    // Only keep the first k layers
    nitro::CbdImage result(img.width(), img.height(), k);
    int width = img.width();
    int height = img.width();
    const auto &data = img.constData();
    auto &outData = result.data();
    // TODO: binary search?
    if (img.isIndexed()) {
        auto cols = img.getColTransform();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int newVal = findClosestValue(newCols, cols[data.get(x, y)]);
                outData.set(x, y, newVal);
            }
        }
    } else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int newVal = findClosestValue(newCols, data.get(x, y));
                outData.set(x, y, newVal);
            }
        }
    }
    result.setIndexed(newCols);
    return result;
}