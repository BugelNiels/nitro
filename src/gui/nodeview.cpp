#include "nodeview.h"
#include "nodes/imagesourcedatamodel.hpp"

#include <QtGui/QScreen>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/GraphicsView>
#include <QtWidgets/QApplication>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/NodeDelegateModelRegistry>


static std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels() {
    auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
    ret->registerModel<nitro::ImageSourceDataModel>("Sources");

//    ret->registerModel<NumberDisplayDataModel>("Displays");
//
//    ret->registerModel<AdditionModel>("Operators");
//
//    ret->registerModel<SubtractionModel>("Operators");
//
//    ret->registerModel<MultiplicationModel>("Operators");
//
//    ret->registerModel<DivisionModel>("Operators");

    return ret;
}

NodeView::NodeView(QWidget *parent) : QDockWidget(parent) {
//    ImprocGraphModel *graphModel = new ImprocGraphModel();

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

    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry = registerDataModels();
    auto *dataFlowGraphModel = new QtNodes::DataFlowGraphModel(registry);
    auto scene = new QtNodes::BasicGraphicsScene(*dataFlowGraphModel);

    auto *view = new QtNodes::GraphicsView(scene);
    view->setContextMenuPolicy(Qt::ActionsContextMenu);
    auto createNodeAction = new QAction(QStringLiteral("Image Source"), view);
    QObject::connect(createNodeAction, &QAction::triggered, [view, dataFlowGraphModel]() {
        // Mouse position in scene coordinates.
        QPointF posView = view->mapToScene(view->mapFromGlobal(QCursor::pos()));

        QtNodes::NodeId const newId = dataFlowGraphModel->addNode("ImageSource");
        dataFlowGraphModel->setNodeData(newId, QtNodes::NodeRole::Position, posView);
    });
    view->insertAction(view->actions().front(), createNodeAction);
    view->showNormal();
    this->setWidget(view);
}

NodeView::~NodeView() = default;
