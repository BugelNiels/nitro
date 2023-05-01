#include "tograyscaledatamodel.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>


nitro::ToGrayScaleDataModel::ToGrayScaleDataModel()
        : ImageDataModel(), _displayWrapper(nullptr), _imgLabel(nullptr) {}

unsigned int nitro::ToGrayScaleDataModel::nPorts(QtNodes::PortType portType) const {
    unsigned int result = 1;

    switch (portType) {
        case QtNodes::PortType::In:
            result = 1;
            break;

        case QtNodes::PortType::Out:
            result = 1;

        default:
            break;
    }

    return result;
}

QtNodes::NodeDataType nitro::ToGrayScaleDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex) const {
    return ImageData().type();
}

void nitro::ToGrayScaleDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
    auto imageData = std::dynamic_pointer_cast<ImageData>(data);

    if (!data || imageData == nullptr || !imageData->isValid()) {
        clearImage();
        _result = nullptr;
        Q_EMIT dataUpdated(0);
        return;
    }

    if (portIndex == 0) {
        _in = imageData;

        if (_in->isGrayscaleImg()) {
            _result = _in;
        } else {
            // TODO: add options?
            auto img = std::make_shared<nitro::CbdImage>(_in->colImage()->convertToFormat(QImage::Format_Grayscale8));
            _result = std::make_shared<ImageData>(img);
        }
        updateImage(_result->getDisplayImg());
        Q_EMIT dataUpdated(0);
    }
}


std::shared_ptr<QtNodes::NodeData> nitro::ToGrayScaleDataModel::outData(QtNodes::PortIndex) {
    return _result;
}