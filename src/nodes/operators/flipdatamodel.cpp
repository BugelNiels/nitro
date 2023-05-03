#include "flipdatamodel.hpp"
#include "src/nodes/invaliddata.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>

#include "src/core/operations/flipcompare.hpp"

nitro::FlipDataModel::FlipDataModel() = default;


unsigned int nitro::FlipDataModel::nPorts(QtNodes::PortType portType) const {
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

QtNodes::NodeDataType nitro::FlipDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {
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


void nitro::FlipDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
    auto inputImg = std::dynamic_pointer_cast<ImageData>(data);

    if (!data || inputImg == nullptr) {
        clearImage();
        _result = nullptr;
        Q_EMIT dataUpdated(0);
        return;
    }
    // TODO: check that dimensions agree and otherwise invalidate and emit warning
    if (portIndex == 0) {
        _inputImgA = inputImg;
    }
    if (portIndex == 1) {
        _inputImgB = inputImg;
    }
    setFlipResult();
    Q_EMIT dataUpdated(0);
}

void nitro::FlipDataModel::setFlipResult() {
    if (_inputImgA == nullptr || _inputImgB == nullptr || !_inputImgA->isValid() || !_inputImgB->isValid()) {
        return;
    }
    const QImage &inA = _inputImgA->getDisplayImg();
    const QImage &inB = _inputImgB->getDisplayImg();
    
    auto finalRes = nitro::operations::flipCompare(inA, inB);
    updateImage(finalRes);
    // TODO: extract this somewhere?
    auto resPtr = std::make_shared<QImage>(finalRes);
    _result = std::make_shared<nitro::ImageData>(resPtr);
}


std::shared_ptr<QtNodes::NodeData> nitro::FlipDataModel::outData(QtNodes::PortIndex port) {
    return _result;
}

