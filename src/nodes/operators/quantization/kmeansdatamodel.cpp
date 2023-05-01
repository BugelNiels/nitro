#include "kmeansdatamodel.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>


nitro::KMeansDataModel::KMeansDataModel() = default;


void nitro::KMeansDataModel::kValChanged(int val) {
    k = val;
    recompute();
}

void nitro::KMeansDataModel::changeDither(bool toggled) {
    dither = toggled;
    recompute();
}

void nitro::KMeansDataModel::iterValChanged(int val) {
    numIterations = val;
    recompute();
}


QWidget *nitro::KMeansDataModel::initBeforeWidget() {
    auto *wrapper = new QWidget();
    auto *layout = new QGridLayout();

    int rowIdx = 0;

    auto *spinBox = new QSpinBox();
    spinBox->setMinimum(0);
    spinBox->setMaximum(255); // TODO: update based on levels of input
    spinBox->setValue(k);
    connect(spinBox, SIGNAL (valueChanged(int)), this, SLOT(kValChanged(int)));
    layout->addWidget(new QLabel("K:"), rowIdx, 0);
    layout->addWidget(spinBox, rowIdx, 1);
    rowIdx++;

    auto *iterSpinBox = new QSpinBox();
    iterSpinBox->setMinimum(0);
    iterSpinBox->setMaximum(100);
    iterSpinBox->setValue(numIterations);
    connect(iterSpinBox, SIGNAL (valueChanged(int)), this, SLOT(iterValChanged(int)));
    layout->addWidget(new QLabel("Iter:"), rowIdx, 0);
    layout->addWidget(iterSpinBox, rowIdx, 1);
    rowIdx++;

    auto *ditherBox = new QCheckBox("Dither");
    connect(ditherBox, SIGNAL (toggled(bool)), this, SLOT(changeDither(bool)));
    layout->addWidget(ditherBox, rowIdx, 0, 1, 2);
    rowIdx++;

    wrapper->setLayout(layout);
    return wrapper;
}

QWidget *nitro::KMeansDataModel::initAfterWidget() {
    return nullptr;
}

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
        means[i] = i * 255.0f / k;
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

static nitro::CbdImage quantisizeDither(const nitro::CbdImage &img, int k, int iter) {
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

static nitro::CbdImage quantisize(const nitro::CbdImage &img, int k, int iter) {
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
            quantMatrix.set(x, y, newPixel);
        }
    }

    quantisized.setIndexed(newCols);
    return quantisized;
}

std::shared_ptr<nitro::ImageData> nitro::KMeansDataModel::compute(const QImage &inputImg) {
    return compute(nitro::CbdImage(inputImg.convertToFormat(QImage::Format_Grayscale8)));
}

std::shared_ptr<nitro::ImageData> nitro::KMeansDataModel::compute(const nitro::CbdImage &inputImg) {
    if (dither) {
        auto result = quantisizeDither(inputImg, k, numIterations);
        auto resPtr = std::make_shared<nitro::CbdImage>(result);
        return std::make_shared<nitro::ImageData>(resPtr);
    } else {
        auto result = quantisize(inputImg, k, numIterations);
        auto resPtr = std::make_shared<nitro::CbdImage>(result);
        return std::make_shared<nitro::ImageData>(resPtr);
    }
}


