#include "kmeans.hpp"

#include <QDebug>


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

static QVector<int> constructColorTable(const nitro::CbdImage &img,
                                        int k, int iter) {
    const auto &matrix = img.constData();
    int width = matrix.width();
    int height = matrix.height();

    QVector<float> means(k);
    QVector<int> meanCounts(k);
    meanCounts.fill(0);
    for (int i = 0; i < k; i++) {
        means[i] = float(i) * 255.0f / float(k);
    }

    nitro::Matrix<int> meanIndices = nitro::Matrix<int>(width, height);

    for (int i = 0; i < iter; i++) {
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

        for (int j = 0; j < k; j++) {
            if(meanCounts[j] == 0) {
                continue;
            }
            means[j] /= float(meanCounts[j]);
            meanCounts[j] = 0;
        }
    }

    QVector<int> cols(k);
    for (int i = 0; i < k; i++) {
        cols[i] = means[i] + 0.5;
    }
    return cols;
}


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

static inline int findClosestValue(const QVector<int> &newCols, float val) {
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

nitro::CbdImage nitro::operations::kMeansDither(const nitro::CbdImage &img, int k, int iter) {
    const auto &matrix = img.constData();
    int width = matrix.width();
    int height = matrix.height();

    nitro::Matrix<float> errMatrix(width, height);

    for (int i = 0; i < matrix.numElems(); i++) {
        errMatrix.set(i, float(matrix.get(i)));
    }

    nitro::CbdImage quantisized(width, height, k);
    auto &quantMatrix = quantisized.data();
    QVector<int> newCols = constructColorTable(img, k, iter);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float oldPixel = errMatrix.get(x, y);
            int newPixel = findClosestValue(newCols, oldPixel);
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

nitro::CbdImage nitro::operations::kMeans(const nitro::CbdImage &img, int k, int iter) {
    const auto &matrix = img.constData();
    int width = matrix.width();
    int height = matrix.height();

    nitro::CbdImage quantisized(width, height, k);
    auto &quantMatrix = quantisized.data();
    QVector<int> newCols = constructColorTable(img, k, iter);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int oldPixel = matrix.get(x, y);
            int newPixel = findClosestValue(newCols, oldPixel);
//            qDebug() << oldPixel <<newPixel << newCols[newPixel] ;
            quantMatrix.set(x, y, newPixel);
        }
    }

    quantisized.setIndexed(newCols);
    return quantisized;
}