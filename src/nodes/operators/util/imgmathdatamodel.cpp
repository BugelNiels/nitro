#include "imgmathdatamodel.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>

#include "src/core/operations/imgmath.hpp"

nitro::ImgMathDataModel::ImgMathDataModel() = default;

void nitro::ImgMathDataModel::modeChanged(int mode) {
    _mode = static_cast<ImgMathMode>(mode);
    recompute();
}

void nitro::ImgMathDataModel::mathValChanged() {
    mathVal = valSpinBox->value();
    recompute();
}

QWidget *nitro::ImgMathDataModel::initBeforeWidget() {
    auto *wrapper = new QWidget();
    auto *horLayout = new QVBoxLayout();

    modeCombobox = new QComboBox();
    // TODO: cleaner
    modeCombobox->addItem("Multiply");
    modeCombobox->addItem("Divide");
    modeCombobox->addItem("Add");
    modeCombobox->addItem("Subtract");
    connect(modeCombobox, SIGNAL (currentIndexChanged(int)), this, SLOT(modeChanged(int)));
    horLayout->addWidget(modeCombobox);

    valSpinBox = new QDoubleSpinBox();
    valSpinBox->setMinimum(0);
    valSpinBox->setMaximum(100000); // TODO: temp
    valSpinBox->setSingleStep(0.1);
    valSpinBox->setLocale(QLocale::English); // Ensure .
    valSpinBox->setValue(mathVal);
    connect(valSpinBox, SIGNAL (editingFinished()), this, SLOT(mathValChanged()));
    horLayout->addWidget(valSpinBox);

    wrapper->setLayout(horLayout);
    return wrapper;
}


std::shared_ptr<nitro::ImageData> nitro::ImgMathDataModel::compute(const QImage &inputImg) {
    return compute(nitro::CbdImage(inputImg.convertToFormat(QImage::Format_Grayscale8)));
}

std::shared_ptr<nitro::ImageData> nitro::ImgMathDataModel::compute(const nitro::CbdImage &inputImg) {

    // In case of addition/subtraction, work on a scale of 0-1
    int modifyVal = int(std::round(mathVal * inputImg.numLevels()));
    CbdImage result;
    switch (_mode) {
        case MULTIPLY:
            result = nitro::operations::multiplyImage(inputImg, mathVal);
            break;
        case DIVIDE:
            result = nitro::operations::divideImage(inputImg, mathVal);
            break;
        case ADD:
            result = nitro::operations::addImage(inputImg, modifyVal);
            break;
        case SUBTRACT:
            result = nitro::operations::subtractImage(inputImg, modifyVal);
            break;
        default:
            return nullptr;
    }
    auto ptrRes = std::make_shared<nitro::CbdImage>(result);
    return std::make_shared<nitro::ImageData>(ptrRes);
}

QJsonObject nitro::ImgMathDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

    modelJson["mathVal"] = mathVal;
    modelJson["mode"] = _mode;

    return modelJson;
}

void nitro::ImgMathDataModel::load(const QJsonObject &p) {
    QJsonValue jVal = p["mathVal"];
    if (!jVal.isUndefined()) {
        mathVal = jVal.toDouble();
        if (valSpinBox) {
            valSpinBox->setValue(mathVal);
        }
    }
    QJsonValue jMode = p["mode"];
    if (!jMode.isUndefined()) {
        _mode = static_cast<ImgMathMode>(jMode.toInt());
        if (modeCombobox) {
            modeCombobox->setCurrentIndex(_mode);
        }
    }
}


