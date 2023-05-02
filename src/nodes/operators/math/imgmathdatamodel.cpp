#include "imgmathdatamodel.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>


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

QWidget *nitro::ImgMathDataModel::initAfterWidget() {
    return nullptr;
}

std::shared_ptr<nitro::ImageData> nitro::ImgMathDataModel::compute(const QImage &inputImg) {
    return compute(nitro::CbdImage(inputImg.convertToFormat(QImage::Format_Grayscale8)));
}

static inline int clampConvert(double a, int k) {
    a += 0.5;
    a = a < 0 ? 0 : a;
    a = a > k ? k : a;
    return int(a);
}

std::shared_ptr<nitro::ImageData> nitro::ImgMathDataModel::compute(const nitro::CbdImage &inputImg) {
    int width = inputImg.width();
    int height = inputImg.height();
    nitro::CbdImage result(width, height, inputImg.numLevels());

    auto &inData = inputImg.constData();
    auto &outData = result.data();

    // In case of addition/division, work on a scale of 0-1
    double modifyVal = mathVal * inputImg.numLevels();

    if (inputImg.isIndexed()) {
        QVector<int> outCols(inputImg.numLevels());
        const auto &inCols = inputImg.getColTransform();
        for (int i = 0; i < inputImg.numLevels(); i++) {
            int val = inCols[i];
            double output = val;
            switch (_mode) {
                case MULTIPLY:
                    output *= mathVal;
                    break;
                case DIVIDE:
                    output /= mathVal;
                    break;
                case ADD:
                    output += modifyVal;
                    break;
                case SUBTRACT:
                    output -= modifyVal;
                    break;
                default:
                    break;
            }
            outCols[i] = clampConvert(output, inputImg.numLevels() - 1);
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    outData.set(x, y, inData.get(x, y));
                }
            }
        }
        result.setIndexed(outCols);
    } else {

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int val = inData.get(x, y);
                double output = val;
                switch (_mode) {
                    case MULTIPLY:
                        output *= mathVal;
                        break;
                    case DIVIDE:
                        output /= mathVal;
                        break;
                    case ADD:
                        output += modifyVal;
                        break;
                    case SUBTRACT:
                        output -= modifyVal;
                        break;
                    default:
                        break;
                }
                outData.set(x, y, clampConvert(output, inputImg.numLevels() - 1));
            }
        }
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


