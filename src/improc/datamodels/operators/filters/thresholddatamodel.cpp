#include "thresholddatamodel.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>

#include "src/improc/core/algorithms/util/threshold.hpp"


nitro::ThresholdDataModel::ThresholdDataModel() = default;

void nitro::ThresholdDataModel::modeChanged(int mode) {
    _mode = mode;
    recompute();
}

void nitro::ThresholdDataModel::thresholdValChanged() {
    threshold = thresholdSpinBox->value();
    recompute();
}

QWidget *nitro::ThresholdDataModel::initBeforeWidget() {
    auto *wrapper = new QWidget();
    auto *horLayout = new QHBoxLayout();

    modeCombobox = new QComboBox();
    modeCombobox->addItem(">");
    modeCombobox->addItem(">=");
    modeCombobox->addItem("<");
    modeCombobox->addItem("<=");
    connect(modeCombobox, SIGNAL (currentIndexChanged(int)), this, SLOT(modeChanged(int)));
    horLayout->addWidget(modeCombobox);

    thresholdSpinBox = new QSpinBox();
    thresholdSpinBox->setMinimum(0);
    thresholdSpinBox->setMaximum(256); // TODO: update based on levels of input
    thresholdSpinBox->setValue(threshold);
    connect(thresholdSpinBox, SIGNAL (editingFinished()), this, SLOT(thresholdValChanged()));
    horLayout->addWidget(thresholdSpinBox);

    wrapper->setLayout(horLayout);
    return wrapper;
}

QWidget *nitro::ThresholdDataModel::initAfterWidget() {
    return nullptr;
}

std::shared_ptr<nitro::ImageData> nitro::ThresholdDataModel::compute(const QImage &inputImg) {
    return compute(nitro::CbdImage(inputImg.convertToFormat(QImage::Format_Grayscale8)));
}

std::shared_ptr<nitro::ImageData> nitro::ThresholdDataModel::compute(const nitro::CbdImage &inputImg) {
    bool greater = (_mode <= 1);
    if (_mode == 1) {
        threshold -= 1;
    } else if (_mode == 3) {
        threshold += 1;
    }

    auto result = nitro::operations::threshold(inputImg, threshold, greater);
    auto ptrRes = std::make_shared<nitro::CbdImage>(result);
    return std::make_shared<nitro::ImageData>(ptrRes);
}

QJsonObject nitro::ThresholdDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

    modelJson["threshold"] = threshold;
    modelJson["mode"] = _mode;

    return modelJson;
}

void nitro::ThresholdDataModel::load(const QJsonObject &p) {
    QJsonValue jThresh = p["threshold"];
    if (!jThresh.isUndefined()) {
        threshold = jThresh.toInt();
        if (thresholdSpinBox) {
            thresholdSpinBox->setValue(threshold);
        }
    }
    QJsonValue jMode = p["mode"];
    if (!jMode.isUndefined()) {
        _mode = jMode.toInt();
        if (modeCombobox) {
            modeCombobox->setCurrentIndex(_mode);
        }
    }
}


