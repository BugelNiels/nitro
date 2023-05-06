#include "tograyscaledatamodel.hpp"
#include "src/nodes/invaliddata.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>

#include <src/core/operations/conversions/grayscaleconvert.hpp>

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

QtNodes::NodeDataType
nitro::ToGrayScaleDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {
    switch (portType) {
        case QtNodes::PortType::In:
            if (index == 0) {
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
            auto img = std::make_shared<nitro::CbdImage>(nitro::operations::toGrayScale(*_in->colImage()));
            _result = std::make_shared<ImageData>(img);
        }
        updateImage(_result->getDisplayImg());
        Q_EMIT dataUpdated(0);
    }
}


std::shared_ptr<QtNodes::NodeData> nitro::ToGrayScaleDataModel::outData(QtNodes::PortIndex) {
    return _result;
}