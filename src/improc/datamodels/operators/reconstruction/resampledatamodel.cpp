#include "resampledatamodel.hpp"
#include "core/algorithms/resampling/linearresampler.hpp"
#include "core/algorithms/resampling/cubichermiteresampler.hpp"
#include "core/algorithms/resampling/cubicinterpolatoryresampler.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>


nitro::ResampleDataModel::ResampleDataModel() = default;



void nitro::ResampleDataModel::modeChanged(int mode) {
    _mode = static_cast<SampleMethod>(mode);
    recompute();
}

void nitro::ResampleDataModel::targetValChanged() {
    targetK = targetSpinBox->value();
    recompute();
}

QWidget *nitro::ResampleDataModel::initBeforeWidget() {
    auto *wrapper = new QWidget();
    auto *layout = new QGridLayout();
    int rowIdx = 0;

    modeCombobox = new QComboBox();
    // TODO: extract this somewhere
    modeCombobox->addItem("Linear");
    modeCombobox->addItem("Cubic Hermite");
    modeCombobox->addItem("Cubic Interpolatory");
    connect(modeCombobox, SIGNAL (currentIndexChanged(int)), this, SLOT(modeChanged(int)));
    layout->addWidget(modeCombobox, rowIdx, 0, 1, 2);
    rowIdx++;

    targetSpinBox = new QSpinBox();
    targetSpinBox->setMinimum(1);
    targetSpinBox->setMaximum(256);
    targetSpinBox->setValue(targetK);
    connect(targetSpinBox, SIGNAL (editingFinished()), this, SLOT(targetValChanged()));
    layout->addWidget(new QLabel("k:"), rowIdx, 0);
    layout->addWidget(targetSpinBox, rowIdx, 1);
    rowIdx++;

    wrapper->setLayout(layout);
    return wrapper;
}

QWidget *nitro::ResampleDataModel::initAfterWidget() {
    return nullptr;
}

std::shared_ptr<nitro::ImageData> nitro::ResampleDataModel::compute(const QImage &inputImg) {
    return compute(nitro::CbdImage(inputImg.convertToFormat(QImage::Format_Grayscale8)));
}

std::shared_ptr<nitro::ImageData> nitro::ResampleDataModel::compute(const nitro::CbdImage &inputImg) {

    Resampler *r;
    switch (_mode) {
        case SampleMethod::LINEAR:
            r = new nitro::LinearSampler();
            break;
        case SampleMethod::CUBIC_HERMITE:
            r = new nitro::CubicHermiteSampler();
            break;
        case SampleMethod::CUBIC_INTERPOLATORY:
            r = new nitro::CubicInterpolatorySampler();
            break;
    }
    if (field == nullptr) {
        field = new nitro::DistanceField(inputImg);
    }
    auto result = r->resample(inputImg, *field, targetK);
    delete r;
    auto ptrRes = std::make_shared<nitro::CbdImage>(result);
    return std::make_shared<nitro::ImageData>(ptrRes);
}

QJsonObject nitro::ResampleDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

    modelJson["target"] = targetK;
    modelJson["mode"] = _mode;

    return modelJson;
}

void nitro::ResampleDataModel::load(const QJsonObject &p) {
    QJsonValue jThresh = p["target"];
    if (!jThresh.isUndefined()) {
        targetK = jThresh.toInt();
        if (targetSpinBox) {
            targetSpinBox->setValue(targetK);
        }
    }
    QJsonValue jMode = p["mode"];
    if (!jMode.isUndefined()) {
        _mode = static_cast<SampleMethod>(jMode.toInt());
        if (modeCombobox) {
            modeCombobox->setCurrentIndex(_mode);
        }
    }
}

void nitro::ResampleDataModel::clearData() {
    field = nullptr;
}
