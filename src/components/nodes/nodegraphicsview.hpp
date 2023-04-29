#pragma once

#include <QtNodes/GraphicsView>
#include <QMenu>
#include "QtNodes/DataFlowGraphModel"

namespace nitro {

    class NodeGraphicsView : public QtNodes::GraphicsView {
    public:
        NodeGraphicsView(QWidget *parent = Q_NULLPTR);

        NodeGraphicsView(QtNodes::BasicGraphicsScene *scene, QtNodes::DataFlowGraphModel* model, QWidget *parent = Q_NULLPTR);

        NodeGraphicsView(const GraphicsView &) = delete;

        NodeGraphicsView operator=(const GraphicsView &) = delete;


    public Q_SLOTS:

    private:
        QtNodes::DataFlowGraphModel* _dataModel;

        QMenu* initNodeMenu();

        QMenu* _nodeMenu{};

        void spawnNodeMenu();

        QMenu *initInputSubMenu();

        QMenu *initConversionsSubMenu();

        QMenu *initOperationsSubMenu();

        QAction *spawnNodeAction(const QString &menuName, const QString &nodeType);
    };

} // nitro
