#include "imopdatamodel.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>


nitro::ImOpDataModel::ImOpDataModel()
        : _displayWrapper(nullptr), _imgLabel(nullptr) {}

QJsonObject nitro::ImOpDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

//    modelJson["image"] = QString::number(_number->number());
// TODO: implement this properly
    return modelJson;
}

void nitro::ImOpDataModel::load(QJsonObject const &p) {
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

    if (portIndex == 0) {
        auto img = compute(inputImg->image());
        _result = std::make_shared<ImageData>(img);

        const QPixmap &p = QPixmap::fromImage(_result->image().getDisplayImg());
        _imgLabel->setPixmap(p.scaled(_embedImgSize, _embedImgSize, Qt::KeepAspectRatio));
        _imgLabel->setMaximumSize(_imgLabel->sizeHint());
        Q_EMIT dataUpdated(0);
    }


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

        _displayWrapper->setStyleSheet("background-color: rgba(0,0,0,0)");
    }

    return _displayWrapper;
}
