#include "slr.hpp"

#include <QDebug>
#include "src/improc/core/algorithms/util/histogram.hpp"
#include "src/improc/core/algorithms/connectedcomps.hpp"

typedef struct IndexedVal {
    int val;
    int idx;
} IndexedVal;

static nitro::CbdImage removeLayers(const nitro::CbdImage &image, const QVector<int> &cols) {
    int width = image.width();
    int height = image.height();
    nitro::CbdImage res(width, height, 256);
    auto &data = res.data();
    const auto &inData = image.constData();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int val = inData.get(x, y);
            int i;
            for (i = 0; i < cols.size(); i++) { // TODO: round?
                if (val < cols[i]) {
                    data.set(x, y, i);
                    break;
                }
            }
            if (i == cols.size()) {
                data.set(x, y, cols.size() - 1);
            }
        }
    }
    res.setIndexed(cols);
    return res;
}

std::vector<int> findPeaks(const std::vector<int> hist, int k) {
    std::vector<IndexedVal> peaks;

    // Find local maxima
    for (int i = 1; i < hist.size() - 1; i++) {
        if (hist[i] > hist[i-1] && hist[i] >= hist[i+1]) {
            peaks.push_back({ hist[i], i });
        }
    }

    // Sort peaks by value
    std::sort(peaks.begin(), peaks.end(), [](const IndexedVal &a, const IndexedVal &b) {
        return a.val > b.val;
    });

    // Return the top k peaks
    std::vector<int> result;
    for (int i = 0; i < std::min(k, (int)peaks.size()); i++) {
        result.push_back(peaks[i].idx);
    }

    return result;
}

nitro::CbdImage nitro::operations::histLayerRemoval(const nitro::CbdImage &img, int k) {
    // Calculate how many new pixels every layer introduces
    // keep the layers that introduce the most pixels

    QVector<int> hist = getHistogram(img);
    QVector<IndexedVal> producedPixels(hist.size());
    for (int i = 0; i < hist.size(); i++) {
        producedPixels[i] = {hist[i], i};
    }

    std::sort(producedPixels.begin(), producedPixels.end(), [](const IndexedVal &a, const IndexedVal &b) {
        return a.val > b.val;
    });


    QVector<int> newLayers(k);
    for (int i = 0; i < k; i++) {
        newLayers[i] = producedPixels[i].idx;
    }
    std::sort(newLayers.begin(), newLayers.end(), [](int a, int b) {
        return a < b;
    });

    return removeLayers(img, newLayers);

}

nitro::CbdImage nitro::operations::componentLayerRemoval(const nitro::CbdImage &img, int k) {
    // Calculate how many new pixels every layer introduces
    // keep the layers that introduce the most pixels

    std::vector<int> hist = nitro::operations::componentHistogram(img);
    qDebug() << hist;

    // TODO: read 1s until this increases, that is where we need to start searching
    // Read 0s from the end until we start seeing 1s, that is the highest white level


    for (int i = hist.size() - 1; i > 0; i--) {
        // change in components
        hist[i] = std::abs(hist[i] - hist[i - 1]);
    }
    hist[0] = 1;
    qDebug() << hist;

    // Alternative TODO: count the sum of the above. Then divide by the desired number of levels, these are the bins
    // Then, distribute these things into bins; grey value is median value of the bin?

    hist = findPeaks(hist, k);

    QVector<int> newLayers(k);
    for (int i = 0; i < k; i++) {
        newLayers[i] = hist[i];
    }
    std::sort(newLayers.begin(), newLayers.end(), [](int a, int b) {
        return a < b;
    });
    qDebug() << newLayers;

    return removeLayers(img, newLayers);

}