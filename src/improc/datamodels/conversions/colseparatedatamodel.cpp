#include "colseparatedatamodel.hpp"

#include <QtGui/QDoubleValidator>
#include <QImageReader>
#include <QVBoxLayout>
#include "3rdparty/nodeeditor/include/QtNodes/InvalidData.hpp"

nitro::ColorSeparateDataModel::ColorSeparateDataModel(int numChannels) {
    _result.resize(numChannels);
}


unsigned int nitro::ColorSeparateDataModel::nPorts(QtNodes::PortType portType) const {
    unsigned int result = 1;

    switch (portType) {
        case QtNodes::PortType::In:
            result = 1;
            break;

        case QtNodes::PortType::Out:
            result = _result.size();

        default:
            break;
    }

    return result;
}

QtNodes::NodeDataType
nitro::ColorSeparateDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {
    switch (portType) {
        case QtNodes::PortType::In:
            if (index == 0) {
                return ImageData().type();
            }
            break;

        case QtNodes::PortType::Out:
            if (index < _result.size()) {
                return ImageData().type();
            }

        default:
            break;
    }
    return QtNodes::InvalidData().type();
}

void nitro::ColorSeparateDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
    auto inputImg = std::dynamic_pointer_cast<ImageData>(data);

    if (!data || !inputImg || !inputImg->isValid()) {
        auto numChannels = _result.size();
        _result.clear();
        _result.resize(numChannels);
        for (int i = 0; i < _result.size(); i++) {
            Q_EMIT dataUpdated(i);
        }
        return;
    }
    _input = inputImg;
    if (portIndex == 0) {
        if (_input->isColImg()) {
            separate(*_input->colImage());
        } else {
            for (auto &i: _result) {
                i = _input;
            }
        }
        for (int i = 0; i < _result.size(); i++) {
            Q_EMIT dataUpdated(i);
        }
    }
}


std::shared_ptr<QtNodes::NodeData> nitro::ColorSeparateDataModel::outData(QtNodes::PortIndex index) {
    return _result[index];
}
