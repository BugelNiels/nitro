#include "greyimagesourcedatamodel.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QFileDialog>
#include <QImageReader>
#include <QVBoxLayout>

nitro::GreyImageSourceDataModel::GreyImageSourceDataModel()
        : _image(std::make_shared<ImageData>()), _displayWrapper(nullptr), _loadButton{nullptr},
          _imgLabel(nullptr) {}

QJsonObject nitro::GreyImageSourceDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

//    modelJson["image"] = QString::number(_number->number());
// TODO: implement this properly
    return modelJson;
}

void nitro::GreyImageSourceDataModel::load(QJsonObject const &p) {
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

unsigned int nitro::GreyImageSourceDataModel::nPorts(QtNodes::PortType portType) const {
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

QtNodes::NodeDataType nitro::GreyImageSourceDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const {
    return ImageData().type();
}

std::shared_ptr<QtNodes::NodeData> nitro::GreyImageSourceDataModel::outData(QtNodes::PortIndex) {
    return _image;
}

QWidget *nitro::GreyImageSourceDataModel::embeddedWidget() {
    if (!_displayWrapper) {
        _displayWrapper = new QWidget();
        auto *layout = new QVBoxLayout(_displayWrapper);

        _loadButton = new QPushButton("Load Image");
        _loadButton->setMaximumSize(_loadButton->sizeHint());
        connect(_loadButton, &QPushButton::pressed, this, &GreyImageSourceDataModel::onLoadButtonPressed);

        _imgLabel = new QLabel("");
        _imgLabel->setFixedSize(_embedImgSize, _embedImgSize);
        _imgLabel->setMaximumSize(_imgLabel->sizeHint());

        layout->addWidget(_loadButton);
        layout->addWidget(_imgLabel);
        layout->setAlignment(_loadButton, Qt::AlignCenter);
        layout->setAlignment(_imgLabel, Qt::AlignCenter);
        // TODO: Temporary ugliness fix
        _displayWrapper->setStyleSheet("background-color: rgba(0,0,0,0)");

    }

    return _displayWrapper;
}

void nitro::GreyImageSourceDataModel::onLoadButtonPressed() {
    QString filePath = QFileDialog::getOpenFileName(
            nullptr, "Load Image", "../images/",
            tr("Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm *ppm)"));

    QImageReader reader(filePath);
    reader.setAutoTransform(true);
    QImage img = reader.read();
    img = img.convertToFormat(QImage::Format_Grayscale8);
    if (img.isNull()) {
        return;
    }

    nitro::CbdImage cbdimg(img);
    _image = std::make_shared<ImageData>(cbdimg);

    const QPixmap &p = QPixmap::fromImage(img);
    _imgLabel->setPixmap(p.scaled(_embedImgSize, _embedImgSize, Qt::KeepAspectRatio));
    _imgLabel->setMaximumSize(_imgLabel->sizeHint());
    Q_EMIT dataUpdated(0);
}
