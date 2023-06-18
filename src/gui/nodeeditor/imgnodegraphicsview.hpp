#pragma once

#include "QtNodes/GraphicsView"
#include <QMenu>
#include "QtNodes/DataFlowGraphModel"

#include "QtNodes/NodeGraphicsView.hpp"
#include "QtNodes/internal/AbstractNodeGeometry.hpp"
#include "QtNodes/NodeInfo.hpp"
#include "nodes/noderegistry.hpp"

namespace nitro {

    class ImageNodeGraphicsView : public NodeGraphicsView {
    public:

        // TODO: better initialization
        ImageNodeGraphicsView(NodeRegistry *nodes, QtNodes::BasicGraphicsScene *scene,
                              QtNodes::DataFlowGraphModel *model,
                              QWidget *parent);


        void mousePressEvent(QMouseEvent *event) override;

        void mouseDoubleClickEvent(QMouseEvent *event) override;

        QMenu *initNodeMenu();

    public Q_SLOTS:


    private:
        QtNodes::NodeId nodeBeingViewed_;
        QtNodes::PortIndex currentPort_ = 0;
        QtNodes::AbstractNodeGeometry &nodeGeometry_;
        NodeRegistry *nodes_;


        QAction *spawnNodeAction(const QtNodes::NodeInfo &info);

        void spawnViewerNodeAt(int x, int y);
    };

} // nitro
