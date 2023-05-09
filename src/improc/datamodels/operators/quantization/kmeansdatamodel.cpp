#include "kmeansdatamodel.hpp"
#include "core/algorithms/quantization/kmeans.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>


nitro::KMeansDataModel::KMeansDataModel() = default;


void nitro::KMeansDataModel::kValChanged() {
    k = kSpinBox->value();
    recompute();
}

void nitro::KMeansDataModel::changeDither(bool toggled) {
    dither = toggled;
    recompute();
}

void nitro::KMeansDataModel::iterValChanged() {
    numIterations = iterSpinBox->value();
    recompute();
}


QWidget *nitro::KMeansDataModel::initBeforeWidget() {
    auto *wrapper = new QWidget();
    auto *layout = new QGridLayout();

    int rowIdx = 0;

    kSpinBox = new QSpinBox();
    kSpinBox->setMinimum(0);
    kSpinBox->setMaximum(256);
    kSpinBox->setValue(k);
    connect(kSpinBox, SIGNAL (editingFinished()), this, SLOT(kValChanged()));
    layout->addWidget(new QLabel("k:"), rowIdx, 0);
    layout->addWidget(kSpinBox, rowIdx, 1);
    rowIdx++;

    iterSpinBox = new QSpinBox();
    iterSpinBox->setMinimum(0);
    iterSpinBox->setMaximum(100);
    iterSpinBox->setValue(numIterations);
    connect(iterSpinBox, SIGNAL (editingFinished()), this, SLOT(iterValChanged()));
    layout->addWidget(new QLabel("Iter:"), rowIdx, 0);
    layout->addWidget(iterSpinBox, rowIdx, 1);
    rowIdx++;

    ditherCheckBox = new QCheckBox("Dither");
    connect(ditherCheckBox, SIGNAL (toggled(bool)), this, SLOT(changeDither(bool)));
    layout->addWidget(ditherCheckBox, rowIdx, 0, 1, 2);
    rowIdx++;

    wrapper->setLayout(layout);
    return wrapper;
}

std::shared_ptr<nitro::ImageData> nitro::KMeansDataModel::compute(const QImage &inputImg) {
    return compute(nitro::CbdImage(inputImg.convertToFormat(QImage::Format_Grayscale8)));
}

std::shared_ptr<nitro::ImageData> nitro::KMeansDataModel::compute(const nitro::CbdImage &inputImg) {
    if (dither) {
        auto result = nitro::operations::kMeansDither(inputImg, k, numIterations);
        auto resPtr = std::make_shared<nitro::CbdImage>(result);
        return std::make_shared<nitro::ImageData>(resPtr);
    } else {
        auto result = nitro::operations::kMeans(inputImg, k, numIterations);
        auto resPtr = std::make_shared<nitro::CbdImage>(result);
        return std::make_shared<nitro::ImageData>(resPtr);
    }
}

QJsonObject nitro::KMeansDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

    modelJson["numIterations"] = numIterations;
    modelJson["dither"] = dither;
    modelJson["k"] = k;

    return modelJson;
}

void nitro::KMeansDataModel::load(const QJsonObject &p) {
    QJsonValue jIter = p["numIterations"];
    if (!jIter.isUndefined()) {
        numIterations = jIter.toInt();
        if (iterSpinBox) {
            iterSpinBox->setValue(numIterations);
        }
    }
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


