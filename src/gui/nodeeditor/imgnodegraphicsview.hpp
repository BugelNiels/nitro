#pragma once

#include "external/nodeeditor/include/QtNodes/GraphicsView"
#include <QMenu>
#include "QtNodes/DataFlowGraphModel"

#include "external/nodeeditor/include/QtNodes/NodeGraphicsView.hpp"
#include "QtNodes/internal/AbstractNodeGeometry.hpp"
#include "external/nodeeditor/include/QtNodes/NodeInfo.hpp"
#include "nodes/noderegistry.hpp"

namespace nitro {

    class ImageNodeGraphicsView : public NodeGraphicsView {
    public:

        // TODO: better initialization
        ImageNodeGraphicsView(NodeRegistry *nodes, QtNodes::BasicGraphicsScene *scene,
                              QtNodes::DataFlowGraphModel *model,
                              QWidget *parent);

        ImageNodeGraphicsView(const GraphicsView &) = delete;


        NodeGraphicsView operator=(const GraphicsView &) = delete;


        void mousePressEvent(QMouseEvent *event) override;

        void mouseDoubleClickEvent(QMouseEvent *event) override;

        QMenu *initNodeMenu();

    public Q_SLOTS:


    private:
        QAction *spawnViewNodeAction = nullptr;
        QtNodes::NodeId nodeBeingViewed;
        QtNodes::PortIndex currentPort = 0;
        QtNodes::AbstractNodeGeometry &nodeGeometry;
        NodeRegistry *nodes_;


        QAction *spawnNodeAction(const QtNodes::NodeInfo &info);

        void spawnViewerNodeAt(int x, int y);
    };

} // nitro
