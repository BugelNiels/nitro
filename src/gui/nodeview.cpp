#include "nodeview.h"
#include "nodes/imagesourcedatamodel.hpp"
#include "nodes/tograyscaledatamodel.hpp"
#include "nodes/operators/thresholddatamodel.hpp"
#include "nodes/nodegraphicsview.hpp"
#include "nodes/nodegraphicsscene.hpp"

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
    ret->registerModel<nitro::ToGrayScaleDataModel>("Operators");
    ret->registerModel<nitro::ThresholdDataModel>("Operators");
//
//    ret->registerModel<SubtractionModel>("Operators");
//
//    ret->registerModel<MultiplicationModel>("Operators");
//
//    ret->registerModel<DivisionModel>("Operators");

    return ret;
}

NodeView::NodeView(QWidget *parent) : QDockWidget(parent) {

    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry = registerDataModels();
    auto *dataFlowGraphModel = new QtNodes::DataFlowGraphModel(registry);
    auto scene = new QtNodes::BasicGraphicsScene(*dataFlowGraphModel);
//    auto scene = new nitro::NodeGraphicsScene(*dataFlowGraphModel);
//    auto *view = new QtNodes::GraphicsView(scene);
    auto *view = new nitro::NodeGraphicsView(scene, dataFlowGraphModel);
    view->setContextMenuPolicy(Qt::ActionsContextMenu);

    // TODO: cleaner





    view->showNormal();
    this->setWidget(view);
}

NodeView::~NodeView() = default;
