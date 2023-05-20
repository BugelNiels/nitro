#include "layerselectiondatamodel.hpp"
#include "core/algorithms/quantization/layerselection.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>


nitro::LayerSelectionDataModel::LayerSelectionDataModel() = default;


void nitro::LayerSelectionDataModel::kValChanged() {
    k = kSpinBox->value();
    recompute();
}

QWidget *nitro::LayerSelectionDataModel::initBeforeWidget() {
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

    wrapper->setLayout(layout);
    return wrapper;
}

std::shared_ptr<nitro::ImageData> nitro::LayerSelectionDataModel::compute(const QImage &inputImg) {
    return compute(nitro::CbdImage(inputImg.convertToFormat(QImage::Format_Grayscale8)));
}

std::shared_ptr<nitro::ImageData> nitro::LayerSelectionDataModel::compute(const nitro::CbdImage &inputImg) {
    auto result = nitro::operations::LayerSelection(inputImg, k);
    auto resPtr = std::make_shared<nitro::CbdImage>(result);
    return std::make_shared<nitro::ImageData>(resPtr);

}

QJsonObject nitro::LayerSelectionDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

    modelJson["k"] = k;

    return modelJson;
}

void nitro::LayerSelectionDataModel::load(const QJsonObject &p) {
    QJsonValue jK = p["k"];
    if (!jK.isUndefined()) {
        k = jK.toInt();
        if (kSpinBox) {
            kSpinBox->setValue(k);
        }
    }
}

void nitro::LayerSelectionDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIdx) {
    ImOpDataModel::setInData(data, portIdx);
    auto inputImg = std::dynamic_pointer_cast<ImageData>(data);

    if (!data || !inputImg || !inputImg->isValid()) {
        return;
    }
    if (inputImg->isGrayscaleImg()) {
        kSpinBox->setMaximum(inputImg->image()->numLevels());
    }
}


