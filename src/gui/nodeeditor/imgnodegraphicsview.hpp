#pragma once

#include <QMenu>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/GraphicsView>
#include <QtNodes/NodeGraphicsView.hpp>
#include <QtNodes/NodeInfo.hpp>
#include <QtNodes/internal/AbstractNodeGeometry.hpp>
#include <nodes/noderegistry.hpp>

namespace nitro {

/**
 * A graphics view for the node graph. Comes with a unique context menu and ability to spawn viewer nodes.
 */
class ImageNodeGraphicsView : public NodeGraphicsView {
public:
    /**
     * Creates a new graphics view.
     * @param nodes The node registry.
     * @param scene The graph scene.
     * @param model The graph model.
     * @param parent Parent widget.
     */
    ImageNodeGraphicsView(std::shared_ptr<NodeRegistry> &nodes,
                          QtNodes::BasicGraphicsScene *scene,
                          QtNodes::DataFlowGraphModel *model,
                          QWidget *parent);

    /**
     * Builds a context menu for spawning nodes.
     * @return A context menu for spawning nodes.
     */
    QMenu *initNodeMenu() override;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

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
