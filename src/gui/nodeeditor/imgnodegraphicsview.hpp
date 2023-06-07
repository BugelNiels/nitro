#pragma once

#include "3rdparty/nodeeditor/include/QtNodes/GraphicsView"
#include <QMenu>
#include "QtNodes/DataFlowGraphModel"

#include "3rdparty/nodeeditor/include/QtNodes/NodeGraphicsView.hpp"
#include "QtNodes/internal/AbstractNodeGeometry.hpp"
#include "src/gui/imgviewer/imgviewer.hpp"
#include "3rdparty/nodeeditor/include/QtNodes/NodeInfo.hpp"
#include "src/core/nodes/initialization/nitronodes.hpp"

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

        QMenu *initNodeMenu();

    public Q_SLOTS:


    private:
        ImageViewer *_imViewer;
        QAction *spawnViewNodeAction = nullptr;
        QtNodes::NodeId nodeBeingViewed;
        QtNodes::PortIndex currentPort = 0;
        QtNodes::AbstractNodeGeometry &nodeGeometry;
        NitroNodes *nodes_;



        QAction *spawnNodeAction(const QtNodes::NodeInfo &info);

        QAction *spawnOutputNodeAction();

        void spawnViewerNodeAt(int x, int y);
    };

} // nitro
