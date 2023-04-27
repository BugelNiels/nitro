#include "kmeansquantisizer.hpp"

#include <QDebug>

#define ITERATIONS 20

nitro::KMeansQuantisizer::KMeansQuantisizer() = default;

nitro::KMeansQuantisizer::~KMeansQuantisizer() = default;

static inline int findClosestMean(const QVector<float> &means, int k, int val) {
    int meanIdx = 0;
    float curDistance = std::abs(val - means[0]);
    for (int i = 1; i < k; i++) {
        float dist = std::abs(val - means[i]);
        if (dist < curDistance) {
            curDistance = dist;
            meanIdx = i;
        }
    }
    return meanIdx;
}

QVector<int> nitro::KMeansQuantisizer::constructColorTable(const CbdImage &img,
                                                           int k) const {
    const auto &matrix = img.constData();
    int width = matrix.width();
    int height = matrix.height();

    QVector<float> means(k);
    QVector<int> meanCounts(k);
    meanCounts.fill(0);
    for (int i = 0; i < k; i++) {
        means[i] = i * 255.0f / k;
    }

    Matrix<int> meanIndices = Matrix<int>(width, height);

    for (int i = 0; i < ITERATIONS; i++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int closestMeanIdx = findClosestMean(means, k, matrix.get(x, y));
                meanIndices.set(x, y, closestMeanIdx);
                meanCounts[closestMeanIdx]++;
            }
        }
        // Update means
        means.fill(0);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int val = meanIndices.get(x, y);
                means[val] += matrix.get(x, y);
            }
        }

        for (int i = 0; i < k; i++) {
            means[i] /= float(meanCounts[i]);
            meanCounts[i] = 0;
        }
    }

    QVector<int> cols(k);
    for (int i = 0; i < k; i++) {
        cols[i] = means[i] + 0.5;
    }
    return cols;
}

int nitro::KMeansQuantisizer::findClosestValue(const CbdImage &img,
                                               const QVector<int> &newCols,
                                               int val) const {
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

int nitro::KMeansQuantisizer::findClosestValue(const CbdImage &img,
                                               const QVector<int> &newCols,
                                               float val) const {
    int idx = 0;
    float curDistance = std::abs(val - float(newCols[0]));
    int k = newCols.size();
    for (int i = 1; i < k; i++) {
        float dist = std::abs(val - float(newCols[i]));
        if (dist < curDistance) {
            curDistance = dist;
            idx = i;
        }
    }
    return idx;
}
