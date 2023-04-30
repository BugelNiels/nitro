#include "imagesourcedatamodel.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QFileDialog>
#include <QImageReader>
#include <QVBoxLayout>

nitro::ImageSourceDataModel::ImageSourceDataModel()
        : _image(std::make_shared<ImageData>()), _path(nullptr), _displayWrapper(nullptr), _loadButton{nullptr},
          _imgLabel(nullptr) {}

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

QWidget *nitro::ImageSourceDataModel::embeddedWidget() {
    if (!_displayWrapper) {
        _displayWrapper = new QWidget();
        auto *layout = new QVBoxLayout(_displayWrapper);

        _loadButton = new QPushButton("Load Image");
        _loadButton->setMaximumSize(_loadButton->sizeHint());
        connect(_loadButton, &QPushButton::pressed, this, &ImageSourceDataModel::onLoadButtonPressed);

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
    _imgLabel->setPixmap(p.scaled(_embedImgSize, _embedImgSize, Qt::KeepAspectRatio));
    _imgLabel->setMaximumSize(_imgLabel->sizeHint());
    Q_EMIT dataUpdated(0);
}
