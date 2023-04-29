#include "imopdatamodel.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>


nitro::ImOpDataModel::ImOpDataModel()
        : _displayWrapper(nullptr), _imgLabel(nullptr) {}


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
        _imgLabel->setPixmap(p.scaled(_embedImgSize, _embedImgSize, Qt::KeepAspectRatio));
        _imgLabel->setMaximumSize(_imgLabel->sizeHint());
        Q_EMIT dataUpdated(0);
    }
}

void nitro::ImOpDataModel::recompute() {

        auto img = compute(_input->image());
        _result = std::make_shared<ImageData>(img);

        const QPixmap &p = QPixmap::fromImage(_result->image().getDisplayImg());
        _imgLabel->setPixmap(p.scaled(_embedImgSize, _embedImgSize, Qt::KeepAspectRatio));
        _imgLabel->setMaximumSize(_imgLabel->sizeHint());
        Q_EMIT dataUpdated(0);
}



std::shared_ptr<QtNodes::NodeData> nitro::ImOpDataModel::outData(QtNodes::PortIndex) {
    return _result;
}

QWidget *nitro::ImOpDataModel::embeddedWidget() {
    if (!_displayWrapper) {
        _displayWrapper = new QWidget();
        auto *layout = new QVBoxLayout(_displayWrapper);
        layout->setAlignment(Qt::AlignCenter);

        _imgLabel = new QLabel("");
        _imgLabel->setFixedSize(_embedImgSize, _embedImgSize);

        _imgLabel->setMaximumSize(_imgLabel->sizeHint());
        layout->addWidget(_imgLabel);
        addWidgets(layout);
        _displayWrapper->setStyleSheet("background-color: rgba(0,0,0,0)");
    }

    return _displayWrapper;
}
