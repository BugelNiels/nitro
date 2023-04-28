#include "imagesourcedatamodel.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QFileDialog>
#include <QImageReader>
#include <QVBoxLayout>

nitro::ImageSourceDataModel::ImageSourceDataModel()
        : _image(std::make_shared<ColImageData>()), _displayWrapper(nullptr), _loadButton{nullptr},
          _imgLabel(nullptr) {}

QJsonObject nitro::ImageSourceDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

//    modelJson["image"] = QString::number(_number->number());
// TODO: implement this properly
    return modelJson;
}

void nitro::ImageSourceDataModel::load(QJsonObject const &p) {
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

unsigned int nitro::ImageSourceDataModel::nPorts(QtNodes::PortType portType) const {
    unsigned int result = 1;

    switch (portType) {
        case QtNodes::PortType::In:
            result = 0;
            break;

        case QtNodes::PortType::Out:
            result = 1;

        default:
            break;
    }

    return result;
}

QtNodes::NodeDataType nitro::ImageSourceDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const {
    return ColImageData().type();
}

std::shared_ptr<QtNodes::NodeData> nitro::ImageSourceDataModel::outData(QtNodes::PortIndex) {
    return _image;
}

QWidget *nitro::ImageSourceDataModel::embeddedWidget() {
    if (!_displayWrapper) {
        _displayWrapper = new QWidget();
        auto *layout = new QVBoxLayout(_displayWrapper);
        layout->setAlignment(Qt::AlignCenter);

        _loadButton = new QPushButton("Load Image");
        connect(_loadButton, &QPushButton::pressed, this, &ImageSourceDataModel::onLoadButtonPressed);
        _loadButton->setMaximumSize(_loadButton->sizeHint());
        layout->addWidget(_loadButton);
        _imgLabel = new QLabel("");
        _imgLabel->setFixedSize(_embedImgSize, _embedImgSize);

        _imgLabel->setMaximumSize(_imgLabel->sizeHint());
        layout->addWidget(_imgLabel);
        // TODO: Temporary ugliness fix
        _displayWrapper->setStyleSheet("background-color: rgba(0,0,0,0)");

    }

    return _displayWrapper;
}

void nitro::ImageSourceDataModel::onLoadButtonPressed() {
    QString filePath = QFileDialog::getOpenFileName(
            nullptr, "Load Image", "../images/",
            tr("Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm *ppm)"));

    QImageReader reader(filePath);
    reader.setAutoTransform(true);
    QImage img = reader.read();
    if (img.isNull()) {
        Q_EMIT dataInvalidated(0);
    }
    _image = std::make_shared<ColImageData>(img);

    const QPixmap &p = QPixmap::fromImage(img);
    _imgLabel->setPixmap(p.scaled(_embedImgSize, _embedImgSize, Qt::KeepAspectRatio));
    _imgLabel->setMaximumSize(_imgLabel->sizeHint());
    Q_EMIT dataUpdated(0);
}
