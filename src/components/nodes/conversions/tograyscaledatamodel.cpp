#include "tograyscaledatamodel.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>


nitro::ToGrayScaleDataModel::ToGrayScaleDataModel()
        : _displayWrapper(nullptr), _imgLabel(nullptr) {}

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
    switch (portType) {
        case QtNodes::PortType::In:
            return ImageData().type();
        case QtNodes::PortType::Out:
            return ImageData().type();
        case QtNodes::PortType::None:
            std::cerr << "Something is going wrong with the port type check!";
            exit(EXIT_FAILURE);
            break;
    }
}

void nitro::ToGrayScaleDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
    auto imageData = std::dynamic_pointer_cast<ImageData>(data);

    if (!data) {
        Q_EMIT dataInvalidated(0);
    }
    if(imageData == nullptr) {
        return;
    }

    if (portIndex == 0) {
        _in = imageData;
    }
    if (_in->isGrayscaleImg()) {
        _result = _in;
    } else {
        // TODO: add options?
        nitro::CbdImage img(_in->colImage().convertToFormat(QImage::Format_Grayscale8));
        _result = std::make_shared<ImageData>(img);
    }


    const QPixmap &p = QPixmap::fromImage(_result->image().getDisplayImg());
    _imgLabel->setPixmap(p.scaled(_embedImgSize, _embedImgSize, Qt::KeepAspectRatio));
    _imgLabel->setMaximumSize(_imgLabel->sizeHint());
    Q_EMIT dataUpdated(0);
}


std::shared_ptr<QtNodes::NodeData> nitro::ToGrayScaleDataModel::outData(QtNodes::PortIndex) {
    return _result;
}

QWidget *nitro::ToGrayScaleDataModel::embeddedWidget() {
    if (!_displayWrapper) {
        _displayWrapper = new QWidget();
        auto *layout = new QVBoxLayout(_displayWrapper);
        layout->setAlignment(Qt::AlignCenter);

        _imgLabel = new QLabel("");
        _imgLabel->setFixedSize(_embedImgSize, _embedImgSize);

        _imgLabel->setMaximumSize(_imgLabel->sizeHint());
        layout->addWidget(_imgLabel);

        _displayWrapper->setStyleSheet("background-color: rgba(0,0,0,0)");
    }

    return _displayWrapper;
}
