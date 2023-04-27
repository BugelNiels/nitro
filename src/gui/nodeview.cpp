#include "nodeview.h"

#include <QtGui/QScreen>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/ConnectionStyle>
#include <QtNodes/GraphicsView>
#include <QtNodes/StyleCollection>
#include <QtWidgets/QApplication>

#include "nodes/simplegraphmodel.hpp"

using QtNodes::BasicGraphicsScene;
using QtNodes::ConnectionStyle;
using QtNodes::GraphicsView;
using QtNodes::NodeRole;
using QtNodes::StyleCollection;

NodeView::NodeView(QWidget *parent) : QDockWidget(parent) {
  SimpleGraphModel* graphModel = new SimpleGraphModel();

  // Initialize and connect two nodes.
  //    {
  //        NodeId id1 = graphModel.addNode();
  //        graphModel.setNodeData(id1, NodeRole::Position, QPointF(0, 0));
  //
  //        NodeId id2 = graphModel.addNode();
  //        graphModel.setNodeData(id2, NodeRole::Position, QPointF(300, 300));
  //
  //        graphModel.addConnection(ConnectionId{id1, 0, id2, 0});
  //    }

  auto scene = new BasicGraphicsScene(*graphModel);

  auto *view = new GraphicsView(scene);
  view->setContextMenuPolicy(Qt::ActionsContextMenu);
  auto createNodeAction = new QAction(QStringLiteral("Create Node"), view);
  QObject::connect(createNodeAction, &QAction::triggered, [view, graphModel]() {
    // Mouse position in scene coordinates.
    QPointF posView = view->mapToScene(view->mapFromGlobal(QCursor::pos()));

    NodeId const newId = graphModel->addNode();
    graphModel->setNodeData(newId, NodeRole::Position, posView);
  });
  view->insertAction(view->actions().front(), createNodeAction);
  view->showNormal();
  this->setWidget(view);
}

NodeView::~NodeView() = default;
