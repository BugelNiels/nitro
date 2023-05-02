#pragma once

#include "3rdparty/nodeeditor/include/QtNodes/GraphicsView"
#include <QMenu>
#include "QtNodes/DataFlowGraphModel"
#include "src/gui/imgviewer/imgviewer.hpp"

namespace nitro {

    class NodeGraphicsView : public QtNodes::GraphicsView {
    public:

        NodeGraphicsView(nitro::ImageViewer *viewer, QtNodes::BasicGraphicsScene *scene,
                         QtNodes::DataFlowGraphModel *model,
                         QWidget *parent);

        NodeGraphicsView(const GraphicsView &) = delete;


        NodeGraphicsView operator=(const GraphicsView &) = delete;


        void mousePressEvent(QMouseEvent *event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;

        void setViewerNodeId(QtNodes::NodeId nodeId);
        QMenu *initNodeMenu();

    public Q_SLOTS:


    private:
        QtNodes::DataFlowGraphModel *_dataModel;
        nitro::ImageViewer *_imViewer;
        QAction *spawnViewNodeAction = nullptr;



        QMenu *_nodeMenu{};

        void spawnNodeMenu();

        QMenu *initInputSubMenu();

        QMenu *initColorSubMenu();

        QMenu *initOperationsSubMenu();

        QMenu *initOutputSubMenu();

        QtNodes::NodeId viewerNodeId;

        QAction *spawnViewerNodeAction();

        QMenu *initQuantizationSubMenu();

        QMenu *initResampleSubMenu();

        QtNodes::NodeId nodeIdViewed;
        QtNodes::PortIndex currentPort;

        QAction *spawnNodeAction(const QString &menuName, const QString &nodeType, const QString &iconName);
    };

} // nitro
