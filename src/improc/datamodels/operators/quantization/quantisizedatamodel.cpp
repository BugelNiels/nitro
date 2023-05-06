#include "quantisizedatamodel.hpp"
#include "core/algorithms/quantization/uniformquantization.hpp"

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

std::shared_ptr<nitro::ImageData> nitro::QuantisizeDataModel::compute(const QImage &inputImg) {
    return compute(nitro::CbdImage(inputImg.convertToFormat(QImage::Format_Grayscale8)));
}

std::shared_ptr<nitro::ImageData> nitro::QuantisizeDataModel::compute(const nitro::CbdImage &inputImg) {
    if (dither) {
        auto result = nitro::operations::uniformQuantizationDither(inputImg, k);
        auto resPtr = std::make_shared<nitro::CbdImage>(result);
        return std::make_shared<nitro::ImageData>(resPtr);
    } else {
        auto result = nitro::operations::uniformQuantization(inputImg, k);
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
    if (inputImg->isGrayscaleImg()) {
        kSpinBox->setMaximum(inputImg->image()->numLevels());
    }
}


