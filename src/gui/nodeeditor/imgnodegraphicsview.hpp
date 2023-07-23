#pragma once

#include <QMenu>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/GraphicsView>
#include <QtNodes/NodeGraphicsView.hpp>
#include <QtNodes/NodeInfo.hpp>
#include <QtNodes/internal/AbstractNodeGeometry.hpp>
#include <nodes/noderegistry.hpp>

namespace nitro {

class ImageNodeGraphicsView : public NodeGraphicsView {
public:
    ImageNodeGraphicsView(std::shared_ptr<NodeRegistry> &nodes,
                          QtNodes::BasicGraphicsScene *scene,
                          QtNodes::DataFlowGraphModel *model,
                          QWidget *parent);

    void mousePressEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    QMenu *initNodeMenu() override;

public Q_SLOTS:

    void dragMoveEvent(QDragMoveEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

private:
    QtNodes::NodeId nodeBeingViewed_;
    QtNodes::PortIndex currentPort_ = 0;
    std::shared_ptr<NodeRegistry> &nodes_;

    QAction *spawnNodeAction(const QtNodes::NodeInfo &info);

    void spawnViewerNodeAt(int x, int y);
};

} // namespace nitro
