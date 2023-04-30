#include "imagesourcedatamodel.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QFileDialog>
#include <QImageReader>
#include <QVBoxLayout>
#include <QPainter>

nitro::ImageSourceDataModel::ImageSourceDataModel()
        : _image(std::make_shared<ImageData>()), _path(nullptr), _loadButton{nullptr} {
}

QJsonObject nitro::ImageSourceDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

    if (_path) {
        modelJson["image"] = *_path;
    }
    return modelJson;
}

void nitro::ImageSourceDataModel::load(QJsonObject const &p) {
    QJsonValue v = p["image"];

    if (!v.isUndefined()) {
        QString path = v.toString();
        loadImage(path);
    }
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
    return ImageData().type();
}

std::shared_ptr<QtNodes::NodeData> nitro::ImageSourceDataModel::outData(QtNodes::PortIndex) {
    return _image;
}

QWidget *nitro::ImageSourceDataModel::initBeforeWidget() {
    _loadButton = new QPushButton("Load Image");
    connect(_loadButton, &QPushButton::pressed, this, &ImageSourceDataModel::onLoadButtonPressed);
    _loadButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    return _loadButton;
}

QWidget *nitro::ImageSourceDataModel::initAfterWidget() {
    return nullptr;
}

void nitro::ImageSourceDataModel::onLoadButtonPressed() {
    QString filePath = QFileDialog::getOpenFileName(
            nullptr, "Load Image", "../images/",
            tr("Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm *ppm)"));

    loadImage(filePath);

}

void nitro::ImageSourceDataModel::loadImage(const QString &filePath) {
    QImageReader reader(filePath);
    reader.setAutoTransform(true);
    QImage img = reader.read();

    if (img.isNull()) {
        return;
    }
    if (img.isGrayscale()) {
        CbdImage cbdImg(img);
        _image = std::make_shared<ImageData>(cbdImg);
    } else {
        _image = std::make_shared<ImageData>(img);
    }

    _path = new QString(filePath);


    const QPixmap &p = QPixmap::fromImage(img);
    updateImage(p);
    Q_EMIT dataUpdated(0);
}
