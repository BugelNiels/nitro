#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>

#include "src/improc/core/cbdimage.hpp"
#include "src/gui/nodeeditor/nodedockwidget.hpp"
#include "ui/surfacevis/renderview.hpp"
#include "src/improc/datamodels/datatypes/imagedata.hpp"
#include "src/core/nodes/nodeinfo.hpp"


namespace nitro {
    class SurfaceViewerDataModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

    public:
        SurfaceViewerDataModel();

        virtual ~SurfaceViewerDataModel() {}

    public:
        static NodeInfo nodeInfo() {
            return {"3D Image Viewer",
                    "ImageViewer3D",
                    {99, 28, 28},
                    ":/icons/nodes/surface.png"};
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
        static void setSurfaceViewer(RenderView *viewer);


    private:
        static RenderView *_surfViewer;
        std::shared_ptr<ImageData> _image;

        QWidget *_displayWrapper;
        QLabel *_sizeLabel;
        QLabel *_layersLabel;
    };
}