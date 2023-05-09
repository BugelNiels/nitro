#include "lumcorrectdatamodel.hpp"

#include "invaliddata.hpp"
#include "core/algorithms/conversions/ycbcrconvert.hpp"
#include "core/algorithms/corrections.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>

nitro::LuminanceCorrectionDataModel::LuminanceCorrectionDataModel() = default;

void nitro::LuminanceCorrectionDataModel::modeChanged(int mode) {
    _mode = mode;
    recompute();
}


unsigned int nitro::LuminanceCorrectionDataModel::nPorts(QtNodes::PortType portType) const {
    unsigned int result = 1;

    switch (portType) {
        case QtNodes::PortType::In:
            result = 2;
            break;

        case QtNodes::PortType::Out:
            result = 1;

        default:
            break;
    }

    return result;
}

QtNodes::NodeDataType nitro::LuminanceCorrectionDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {
    switch (portType) {
        case QtNodes::PortType::In:
            if (index < 2) {
                return ImageData().type();
            }
            break;

        case QtNodes::PortType::Out:
            if (index == 0) {
                return ImageData().type();
            }

        default:
            break;
    }
    return nitro::InvalidData().type();
}
QWidget *nitro::LuminanceCorrectionDataModel::initBeforeWidget() {
    auto *wrapper = new QWidget();
    auto *horLayout = new QHBoxLayout();

    modeCombobox = new QComboBox();
    modeCombobox->addItem("Global");
    modeCombobox->addItem("Local");
    connect(modeCombobox, SIGNAL (currentIndexChanged(int)), this, SLOT(modeChanged(int)));
    horLayout->addWidget(modeCombobox);

    wrapper->setLayout(horLayout);
    return wrapper;
}


void nitro::LuminanceCorrectionDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
    auto inputImg = std::dynamic_pointer_cast<ImageData>(data);

    if (!data || !inputImg || !inputImg->isValid()) {
        clearImage();
        _result = nullptr;
        Q_EMIT dataUpdated(0);
        return;
    }
    if (portIndex == 0) {
        _input = inputImg;
    }
    if(portIndex == 1) {
        if(inputImg->isColImg()) {
            auto channels = nitro::operations::separateYCbCr(*inputImg->colImage());
            auto yChannel = CbdImage(channels[0]);
            _inputRef = std::make_shared<ImageData>(std::make_shared<CbdImage>(yChannel));
        } else {
            _inputRef = inputImg;
        }
    }
    if(_input != nullptr && _inputRef != nullptr && _input->isValid() && _inputRef->isValid()) {

        if (_input->isColImg()) {
            _result = colorBrightnessCorrect(*_input->colImage(), *_inputRef->image());
        } else {
            _result = brightnessCorrect(*_input->image(), *_inputRef->image());
        }

        updateImage(_result->getDisplayImg());
        Q_EMIT dataUpdated(0);
    }
}

void nitro::LuminanceCorrectionDataModel::recompute() {

    if(_input == nullptr || _inputRef == nullptr) {
        return;
    }
    if(_input->isValid() && _inputRef->isValid()) {

        if (_input->isColImg()) {
            _result = colorBrightnessCorrect(*_input->colImage(), *_inputRef->image());
        } else {
            _result = brightnessCorrect(*_input->image(), *_inputRef->image());
        }

        updateImage(_result->getDisplayImg());
        Q_EMIT dataUpdated(0);
    }
}


std::shared_ptr<QtNodes::NodeData> nitro::LuminanceCorrectionDataModel::outData(QtNodes::PortIndex) {
    return _result;
}

QJsonObject nitro::LuminanceCorrectionDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();
    modelJson["mode"] = _mode;

    return modelJson;
}

void nitro::LuminanceCorrectionDataModel::load(const QJsonObject &p) {
    QJsonValue jMode = p["mode"];
    if (!jMode.isUndefined()) {
        _mode = jMode.toInt();
        if (modeCombobox) {
            modeCombobox->setCurrentIndex(_mode);
        }
    }
}

std::shared_ptr<nitro::ImageData>
nitro::LuminanceCorrectionDataModel::colorBrightnessCorrect(QImage &image, nitro::CbdImage &image1) {
    // TODO: bit of a dangerous way of using mode
    auto result = nitro::operations::brightnessCorrect(image, image1, modeCombobox->currentIndex());
    return std::make_shared<ImageData>(std::make_shared<QImage>(result));
}

std::shared_ptr<nitro::ImageData>
nitro::LuminanceCorrectionDataModel::brightnessCorrect(nitro::CbdImage &image, nitro::CbdImage &image1) {
    auto result = nitro::operations::brightnessCorrect(image, image1, modeCombobox->currentIndex());
    return std::make_shared<ImageData>(std::make_shared<nitro::CbdImage>(result));
}


