#include "imageviewerdatamodel.hpp"
#include "3rdparty/nodeeditor/include/QtNodes/InvalidData.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QFileDialog>
#include <QImageReader>
#include <QVBoxLayout>


nitro::ImageViewer *nitro::ImageViewerDataModel::_imViewer = nullptr;

nitro::ImageViewerDataModel::ImageViewerDataModel()
        : _image(std::make_shared<ImageData>()), _displayWrapper(nullptr) {
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

QtNodes::NodeDataType
nitro::ImageViewerDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {

    switch (portType) {
        case QtNodes::PortType::In:
            if (index == 0) {
                return ImageData().type();
            }
            break;

        case QtNodes::PortType::Out:

        default:
            break;
    }
    return QtNodes::InvalidData().type();
}

std::shared_ptr<QtNodes::NodeData> nitro::ImageViewerDataModel::outData(QtNodes::PortIndex) {
    return _image;
}

void nitro::ImageViewerDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
    auto inputImg = std::dynamic_pointer_cast<ImageData>(data);
    if (!data || inputImg == nullptr || !inputImg->isValid()) {
        _imViewer->removeImage();
        _sizeLabel->setText("");
        _layersLabel->setText("");
        return;
    }

    if (portIndex == 0) {
        const QImage &img = inputImg->getDisplayImg();
        _imViewer->setImage(img);
        _sizeLabel->setText(QString("%1 x %2").arg(img.width()).arg(img.height()));
        _layersLabel->setText(QString("%1 layers").arg(1 << img.depth()));
    }

}

QWidget *nitro::ImageViewerDataModel::embeddedWidget() {
    if (!_displayWrapper) {
        _displayWrapper = new QWidget();
        _displayWrapper->setAttribute(Qt::WA_TranslucentBackground);
        auto *layout = new QVBoxLayout(_displayWrapper);

        _sizeLabel = new QLabel();
        _layersLabel = new QLabel();
        _sizeLabel->setMinimumWidth(120);
        layout->addWidget(_sizeLabel);
        layout->addWidget(_layersLabel);
        _displayWrapper->setLayout(layout);

    }

    return _displayWrapper;
}


void nitro::ImageViewerDataModel::setViewer(nitro::ImageViewer *viewer) {
    ImageViewerDataModel::_imViewer = viewer;
}
