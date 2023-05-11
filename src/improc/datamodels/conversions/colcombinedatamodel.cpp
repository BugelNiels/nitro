#include "colcombinedatamodel.hpp"

#include <QtGui/QDoubleValidator>
#include <QImageReader>
#include <QVBoxLayout>
#include "src/core/invaliddata.hpp"

nitro::ColorCombineDataModel::ColorCombineDataModel(int numChannels) {
    _input.resize(numChannels);
}


unsigned int nitro::ColorCombineDataModel::nPorts(QtNodes::PortType portType) const {
    unsigned int result = 1;

    switch (portType) {
        case QtNodes::PortType::In:
            result = _input.size();
            break;

        case QtNodes::PortType::Out:
            result = 1;

        default:
            break;
    }

    return result;
}

QtNodes::NodeDataType
nitro::ColorCombineDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {
    switch (portType) {
        case QtNodes::PortType::In:
            if (index < _input.size()) {
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

void nitro::ColorCombineDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
    auto inputImg = std::dynamic_pointer_cast<ImageData>(data);

    if (!data || !inputImg || !inputImg->isValid()) {
        _result = nullptr;
        Q_EMIT dataUpdated(0);
        return;
    }
    if (portIndex < _input.size()) {
        _input[portIndex] = inputImg;
    }

    bool shouldUpdate = true;
    for (const auto &dat: _input) {
        if (dat == nullptr || !dat->isValid()) {
            shouldUpdate = false;
            break;
        }
    }

    if (shouldUpdate) {
        combine(_input);
        Q_EMIT dataUpdated(0);
    }
}


std::shared_ptr<QtNodes::NodeData> nitro::ColorCombineDataModel::outData(QtNodes::PortIndex index) {
    return _result;
}
