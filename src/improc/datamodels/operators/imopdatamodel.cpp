#include "imopdatamodel.hpp"
#include "external/nodeeditor/include/QtNodes/InvalidData.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>


nitro::ImOpDataModel::ImOpDataModel() = default;


unsigned int nitro::ImOpDataModel::nPorts(QtNodes::PortType portType) const {
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

QtNodes::NodeDataType nitro::ImOpDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {
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
    return QtNodes::InvalidData().type();
}

void nitro::ImOpDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
    auto inputImg = std::dynamic_pointer_cast<ImageData>(data);

    if (!data || !inputImg || !inputImg->isValid()) {
        clearImage();
        clearData();
        _result = nullptr;
        Q_EMIT dataUpdated(0);
        return;
    }
    _input = inputImg;
    if (portIndex == 0) {
        clearData();
        if (_input->isColImg()) {
            _result = compute(*_input->colImage());
        } else {
            _result = compute(*_input->image());
        }

        updateImage(_result->getDisplayImg());
        Q_EMIT dataUpdated(0);
    }
}

void nitro::ImOpDataModel::recompute() {

    if (_input == nullptr || !_input->isValid()) {
        return;
    }
    if (_input->isColImg()) {
        _result = compute(*_input->colImage());
    } else {
        _result = compute(*_input->image());
    }

    updateImage(_result->getDisplayImg());
    Q_EMIT dataUpdated(0);
}


std::shared_ptr<QtNodes::NodeData> nitro::ImOpDataModel::outData(QtNodes::PortIndex) {
    return _result;
}

void nitro::ImOpDataModel::clearData() {

}
