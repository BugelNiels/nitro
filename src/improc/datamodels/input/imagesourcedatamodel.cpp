#include "imagesourcedatamodel.hpp"
#include "src/core/invaliddata.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QFileDialog>
#include <QImageReader>
#include <QVBoxLayout>
#include <QPainter>
#include "util/imgresourcereader.hpp"

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

QtNodes::NodeDataType
nitro::ImageSourceDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {
    switch (portType) {
        case QtNodes::PortType::In:
            break;

        case QtNodes::PortType::Out:
            if (index == 0) {
                return ImageData().type();
            }

        default:
            break;
    }
    return nitro::InvalidData().type();
}

std::shared_ptr<QtNodes::NodeData> nitro::ImageSourceDataModel::outData(QtNodes::PortIndex) {
    return _image;
}

QWidget *nitro::ImageSourceDataModel::initBeforeWidget() {
    _loadButton = new QPushButton("Load Image");
    _loadButton->setIcon(nitro::ImgResourceReader::getPixMap(":/icons/folder_open.png"));
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
        auto cbdImg = std::make_shared<nitro::CbdImage>(img);
        _image = std::make_shared<ImageData>(cbdImg);
    } else {
        auto ptrImg = std::make_shared<QImage>(img);
        _image = std::make_shared<ImageData>(ptrImg);
    }

    _path = new QString(filePath);

    // TODO: better
    curNodeName = QFileInfo(filePath).fileName();
    updateImage(img);
    Q_EMIT dataUpdated(0);
}
