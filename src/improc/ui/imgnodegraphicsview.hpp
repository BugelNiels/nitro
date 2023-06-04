#pragma once

#include "3rdparty/nodeeditor/include/QtNodes/GraphicsView"
#include <QMenu>
#include "QtNodes/DataFlowGraphModel"

#include "src/gui/nodeeditor/nodegraphicsview.hpp"
#include "QtNodes/internal/AbstractNodeGeometry.hpp"
#include "ui/imgviewer/imgviewer.hpp"
#include "src/core/nodes/nodeinfo.hpp"
#include "datamodels/nitronodes.hpp"

namespace nitro {

    class ImageNodeGraphicsView : public NodeGraphicsView {
    public:

        // TODO: better initialization
        ImageNodeGraphicsView(NitroNodes *nodes, ImageViewer *viewer, QtNodes::BasicGraphicsScene *scene,
                              QtNodes::DataFlowGraphModel *model,
                              QWidget *parent);

        ImageNodeGraphicsView(const GraphicsView &) = delete;


        NodeGraphicsView operator=(const GraphicsView &) = delete;


        void mousePressEvent(QMouseEvent *event) override;

        void mouseDoubleClickEvent(QMouseEvent *event) override;

        void setViewerNodeId(QtNodes::NodeId nodeId);

        QMenu *initNodeMenu();

    public Q_SLOTS:


    private:
        ImageViewer *_imViewer;
        QAction *spawnViewNodeAction = nullptr;
        QtNodes::NodeId nodeIdViewed;
        QtNodes::PortIndex currentPort = 0;
        QtNodes::AbstractNodeGeometry &nodeGeometry;
        NitroNodes *nodes_;


        QtNodes::NodeId viewerNodeId;


        QAction *spawnNodeAction(const nitro::NodeInfo &info);

        QAction *spawnViewerNodeAction();
    };

} // nitro
