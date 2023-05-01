#include "quantisizedatamodel.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>


nitro::QuantisizeDataModel::QuantisizeDataModel() = default;


void nitro::QuantisizeDataModel::kValChanged(int val) {
    k = val;
    recompute();
}

void nitro::QuantisizeDataModel::changeDither(bool toggled) {
    dither = toggled;
    recompute();
}


QWidget *nitro::QuantisizeDataModel::initBeforeWidget() {
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

    auto *ditherBox = new QCheckBox("Dither");
    connect(ditherBox, SIGNAL (toggled(bool)), this, SLOT(changeDither(bool)));
    layout->addWidget(ditherBox, rowIdx, 0, 1, 2);
    rowIdx++;

    wrapper->setLayout(layout);
    return wrapper;
}

QWidget *nitro::QuantisizeDataModel::initAfterWidget() {
    return nullptr;
}

static inline int findClosestValue(const nitro::CbdImage &img, int k, int val) {
    int dynRange = img.numLevels();
    float proportion = val / float(dynRange);
    int newVal = proportion * (k - 1) + 0.5f;
    return newVal;
}

static inline int findClosestValue(const nitro::CbdImage &img, int k, float val) {
    int dynRange = img.numLevels();
    float proportion = val / float(dynRange);
    int newVal = proportion * (k - 1) + 0.5f;
    return newVal;
}

static nitro::CbdImage quantisizeDither(const nitro::CbdImage &img,
                                        int k) {
    const auto &matrix = img.constData();
    int width = matrix.width();
    int height = matrix.height();

    nitro::Matrix<float> errMatrix(width, height);

    for (int i = 0; i < matrix.numElems(); i++) {
        errMatrix.set(i, float(matrix.get(i)));
    }

    QVector<int> newCols;
    newCols.resize(k);

    for (int i = 0; i < k; i++) {
        newCols[i] = i * 255 / float(k - 1);
    }
    nitro::CbdImage quantisized(width, height, k);
    auto &quantMatrix = quantisized.data();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float oldPixel = errMatrix.get(x, y);
            int newPixel = findClosestValue(img, k, oldPixel);
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

static nitro::CbdImage quantisize(const nitro::CbdImage &img,
                                  int k) {
    const auto &matrix = img.constData();
    int width = matrix.width();
    int height = matrix.height();

    nitro::CbdImage quantisized(width, height, k);
    auto &quantMatrix = quantisized.data();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int oldPixel = matrix.get(x, y);
            int newPixel = findClosestValue(img, k, oldPixel);
            quantMatrix.set(x, y, newPixel);
        }
    }

    QVector<int> newCols;
    newCols.resize(k);

    for (int i = 0; i < k; i++) {
        newCols[i] = i * 255 / float(k - 1);
    }
    quantisized.setIndexed(newCols);
    return quantisized;
}

std::shared_ptr<nitro::ImageData> nitro::QuantisizeDataModel::compute(const QImage &inputImg) {
    return compute(nitro::CbdImage(inputImg.convertToFormat(QImage::Format_Grayscale8)));
}

std::shared_ptr<nitro::ImageData> nitro::QuantisizeDataModel::compute(const nitro::CbdImage &inputImg) {
    if (dither) {
        auto result = quantisizeDither(inputImg, k);
        auto resPtr = std::make_shared<nitro::CbdImage>(result);
        return std::make_shared<nitro::ImageData>(resPtr);
    } else {
        auto result = quantisize(inputImg, k);
        auto resPtr = std::make_shared<nitro::CbdImage>(result);
        return std::make_shared<nitro::ImageData>(resPtr);
    }
}


