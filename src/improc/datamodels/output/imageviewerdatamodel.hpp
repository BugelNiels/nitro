#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>

#include "src/improc/core/cbdimage.hpp"
#include "src/improc/datamodels/imagedata.hpp"
#include "src/gui/nodeeditor/nodedockwidget.hpp"
#include "src/improc/ui/imgviewer/imgviewer.hpp"
#include "src/improc/datamodels/imagedatamodel.hpp"


namespace nitro {
    class ImageViewerDataModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

    public:
        ImageViewerDataModel();

        virtual ~ImageViewerDataModel() {}

    public:
        static NodeInfo nodeInfo() {
            return {"Image Viewer",
                    "ImageViewer",
                    {60, 29, 38},
                    ":/icons/nodes/viewer.png"};
        }

        QString caption() const override { return nodeInfo().getNodeName(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeInfo().getNodeId(); }

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