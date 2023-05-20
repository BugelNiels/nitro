#include "surfaceviewerdatamodel.hpp"
#include "src/core/invaliddata.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>


nitro::RenderView *nitro::SurfaceViewerDataModel::_surfViewer = nullptr;

nitro::SurfaceViewerDataModel::SurfaceViewerDataModel()
        : _image(std::make_shared<ImageData>()), _displayWrapper(nullptr) {
}

unsigned int nitro::SurfaceViewerDataModel::nPorts(QtNodes::PortType portType) const {
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
nitro::SurfaceViewerDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {

    switch (portType) {
        case QtNodes::PortType::In:
            if (index < 2) {
                return ImageData().type();
            }
            break;

        case QtNodes::PortType::Out:

        default:
            break;
    }
    return nitro::InvalidData().type();
}

std::shared_ptr<QtNodes::NodeData> nitro::SurfaceViewerDataModel::outData(QtNodes::PortIndex) {
    return _image;
}

void nitro::SurfaceViewerDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
    auto inputImg = std::dynamic_pointer_cast<ImageData>(data);
    if (!data || inputImg == nullptr || !inputImg->isValid()) {
//        _imViewer->removeImage();
        _sizeLabel->setText("");
        _layersLabel->setText("");
        return;
    }

    if (portIndex == 0) {
        const QImage &img = inputImg->getDisplayImg();
        _surfViewer->updateBuffers(img);
        // TODO: unify input of both the image viewer and the surface viewer
//        _imViewer->setImage(img);
        _sizeLabel->setText(QString("%1 x %2").arg(img.width()).arg(img.height()));
        _layersLabel->setText(QString("%1 layers").arg(1 << img.depth()));
    }

}

QWidget *nitro::SurfaceViewerDataModel::embeddedWidget() {
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


void nitro::SurfaceViewerDataModel::setSurfaceViewer(nitro::RenderView *viewer) {
    SurfaceViewerDataModel::_surfViewer = viewer;
}
