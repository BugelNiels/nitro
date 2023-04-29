#include "imageviewerdatamodel.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QFileDialog>
#include <QImageReader>
#include <QVBoxLayout>


nitro::ImageView *nitro::ImageViewerDataModel::_imViewer = nullptr;

nitro::ImageViewerDataModel::ImageViewerDataModel()
        : _image(std::make_shared<ImageData>()), _displayWrapper(nullptr) {
}

QJsonObject nitro::ImageViewerDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

//    modelJson["image"] = QString::number(_number->number());
// TODO: implement this properly
    return modelJson;
}

void nitro::ImageViewerDataModel::load(QJsonObject const &p) {
    // TODO: implemenet
//    QJsonValue v = p["number"];
//
//    if (!v.isUndefined()) {
//        QString strNum = v.toString();
//
//        bool ok;
//        double d = strNum.toDouble(&ok);
//        if (ok) {
//            _number = std::make_shared<DecimalData>(d);
//
//            if (_lineEdit)
//                _lineEdit->setText(strNum);
//        }
//    }
}

unsigned int nitro::ImageViewerDataModel::nPorts(QtNodes::PortType portType) const {
    unsigned int result = 1;

    switch (portType) {
        case QtNodes::PortType::In:
            result = 1;
            break;

        case QtNodes::PortType::Out:
            result = 0;

        default:
            break;
    }

    return result;
}

QtNodes::NodeDataType nitro::ImageViewerDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const {
    return ImageData().type();
}

std::shared_ptr<QtNodes::NodeData> nitro::ImageViewerDataModel::outData(QtNodes::PortIndex) {
    return _image;
}

void nitro::ImageViewerDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
    auto inputImg = std::dynamic_pointer_cast<ImageData>(data);

    if (!data) {
        return;
    }

    if (portIndex == 0) {
        _imViewer->setImage(inputImg->image().getDisplayImg());
    }


}

QWidget *nitro::ImageViewerDataModel::embeddedWidget() {
    if (!_displayWrapper) {
        _displayWrapper = new QWidget();
        auto *layout = new QVBoxLayout(_displayWrapper);

        // TODO: Temporary ugliness fix
        _displayWrapper->setStyleSheet("background-color: rgba(0,0,0,0)");

    }

    return _displayWrapper;
}


void nitro::ImageViewerDataModel::setViewer(nitro::ImageView *viewer) {
    ImageViewerDataModel::_imViewer = viewer;
}
