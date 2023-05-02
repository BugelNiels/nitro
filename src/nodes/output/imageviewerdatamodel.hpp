#pragma once

#include "3rdparty/nodeeditor/include/QtNodes/NodeDelegateModel"

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>

#include "cbdimage.hpp"
#include "src/nodes/imagedata.hpp"
#include "src/gui/nodeeditor/nodedockwidget.hpp"
#include "src/gui/imgviewer/imgviewer.hpp"
#include "src/nodes/imagedatamodel.hpp"


namespace nitro {
    class ImageViewerDataModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

    public:
        ImageViewerDataModel();

        virtual ~ImageViewerDataModel() {}

    public:
        static QString nodeCaption() { return QStringLiteral("Image Viewer"); }

        static QString nodeName() { return QStringLiteral("ImageViewer"); }
        static QString nodeIcon() { return QStringLiteral(":/icons/nodes/viewer.png"); }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }

    public:
        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

        QWidget *embeddedWidget() override;

    public:
        static void setViewer(nitro::ImageViewer *viewer);


    private:
        std::shared_ptr<ImageData> _image;

        QWidget *_displayWrapper;
        static ImageViewer *_imViewer;
        QLabel *_sizeLabel;
        QLabel *_layersLabel;
    };
}