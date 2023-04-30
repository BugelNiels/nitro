#include "imopdatamodel.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>


nitro::ImOpDataModel::ImOpDataModel() {}


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

QtNodes::NodeDataType nitro::ImOpDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const {
    return ImageData().type();
}

void nitro::ImOpDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
    auto inputImg = std::dynamic_pointer_cast<ImageData>(data);

    if (!data) {
        Q_EMIT dataInvalidated(0);
    }
    _input = inputImg;
    if (portIndex == 0) {
        auto img = compute(inputImg->image());
        _result = std::make_shared<ImageData>(img);

        const QPixmap &p = QPixmap::fromImage(_result->image().getDisplayImg());
        updateImage(p);
        Q_EMIT dataUpdated(0);
    }
}

void nitro::ImOpDataModel::recompute() {

    if(_input == nullptr) {
        return;
    }
    auto img = compute(_input->image());
    _result = std::make_shared<ImageData>(img);

    const QPixmap &p = QPixmap::fromImage(_result->image().getDisplayImg());
    updateImage(p);
    Q_EMIT dataUpdated(0);
}


std::shared_ptr<QtNodes::NodeData> nitro::ImOpDataModel::outData(QtNodes::PortIndex) {
    return _result;
}
