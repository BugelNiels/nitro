#include "quantisizedatamodel.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>


nitro::QuantisizeDataModel::QuantisizeDataModel() = default;


void nitro::QuantisizeDataModel::kValChanged() {
    k = kSpinBox->value();
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

    kSpinBox = new QSpinBox();
    kSpinBox->setMinimum(0);
    kSpinBox->setMaximum(255);
    kSpinBox->setValue(k);
    connect(kSpinBox, SIGNAL (editingFinished()), this, SLOT(kValChanged()));
    layout->addWidget(new QLabel("k:"), rowIdx, 0);
    layout->addWidget(kSpinBox, rowIdx, 1);
    rowIdx++;

    ditherCheckBox = new QCheckBox("Dither");
    connect(ditherCheckBox, SIGNAL (toggled(bool)), this, SLOT(changeDither(bool)));
    layout->addWidget(ditherCheckBox, rowIdx, 0, 1, 2);
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

QJsonObject nitro::QuantisizeDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

    modelJson["dither"] = dither;
    modelJson["k"] = k;

    return modelJson;
}

void nitro::QuantisizeDataModel::load(const QJsonObject &p) {
    QJsonValue jDither = p["dither"];
    if (!jDither.isUndefined()) {
        dither = jDither.toBool();
        if (ditherCheckBox) {
            ditherCheckBox->setChecked(dither);
        }
    }
    QJsonValue jK = p["k"];
    if (!jK.isUndefined()) {
        k = jK.toInt();
        if (kSpinBox) {
            kSpinBox->setValue(k);
        }
    }
}

void nitro::QuantisizeDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIdx) {
    ImOpDataModel::setInData(data, portIdx);
    auto inputImg = std::dynamic_pointer_cast<ImageData>(data);

    if (!data || !inputImg || !inputImg->isValid()) {
        Q_EMIT dataInvalidated(0);
        return;
    }
    if(inputImg->isGrayscaleImg()) {
        kSpinBox->setMaximum(inputImg->image()->numLevels());
    }
}


