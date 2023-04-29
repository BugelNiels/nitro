#pragma once

#include <QtNodes/GraphicsView>
#include <QMenu>
#include "QtNodes/DataFlowGraphModel"
#include "src/gui/imgviewer/imgviewer.hpp"

namespace nitro {

    class NodeGraphicsView : public QtNodes::GraphicsView {
    public:
        NodeGraphicsView(QWidget *parent = Q_NULLPTR);

        NodeGraphicsView(nitro::ImageViewer *viewer, QtNodes::BasicGraphicsScene *scene,
                         QtNodes::DataFlowGraphModel *model,
                         QWidget *parent);

        NodeGraphicsView(const GraphicsView &) = delete;


        NodeGraphicsView operator=(const GraphicsView &) = delete;


    public Q_SLOTS:


    private:
        QtNodes::DataFlowGraphModel *_dataModel;
        nitro::ImageViewer *_imViewer;
        QAction *spawnViewNodeAction = nullptr;

        bool shiftPressed = false;
        bool controlPressed = false;

        QMenu *initNodeMenu();

        QMenu *_nodeMenu{};

        void spawnNodeMenu();

        QMenu *initInputSubMenu();

        QMenu *initConversionsSubMenu();

        QMenu *initOperationsSubMenu();

        QAction *spawnNodeAction(const QString &menuName, const QString &nodeType);

        QMenu *initOutputSubMenu();

        QtNodes::NodeId viewerNodeId;

        QAction *spawnViewerNodeAction();

        void keyPressEvent(QKeyEvent *event) override;

        void keyReleaseEvent(QKeyEvent *event) override;

        void mousePressEvent(QMouseEvent *event) override;
    };

} // nitro
