#include "nodeview.hpp"
#include "src/components/nodes/input/greyimagesourcedatamodel.hpp"
#include "src/components/nodes/input/colimagesourcedatamodel.hpp"
#include "src/components/nodes/conversions/tograyscaledatamodel.hpp"
#include "nodes/operators/thresholddatamodel.hpp"
#include "nodes/nodegraphicsview.hpp"
#include "nodes/output/imageviewerdatamodel.hpp"

#include <QtGui/QScreen>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/GraphicsView>
#include <QtWidgets/QApplication>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/NodeDelegateModelRegistry>


static std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels() {
    auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
    ret->registerModel<nitro::GreyImageSourceDataModel>("Input");
    ret->registerModel<nitro::ColImageSourceDataModel>("Input");
    ret->registerModel<nitro::ImageViewerDataModel>("Output");
    ret->registerModel<nitro::ToGrayScaleDataModel>("Converter");
    ret->registerModel<nitro::ThresholdDataModel>("Operator");

    return ret;
}

nitro::NodeView::NodeView(nitro::ImageViewer *imViewer, QWidget *parent) : QDockWidget(parent),
                                                                           dataFlowGraphModel(nullptr) {
    setWindowTitle("Node Editor");
    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry = registerDataModels();
    dataFlowGraphModel = new QtNodes::DataFlowGraphModel(registry);
    dataFlowGraphModel->addNode(nitro::GreyImageSourceDataModel::nodeName());
    auto scene = new QtNodes::BasicGraphicsScene(*dataFlowGraphModel);
    auto *view = new nitro::NodeGraphicsView(imViewer, scene, dataFlowGraphModel, this);
    view->setContextMenuPolicy(Qt::ActionsContextMenu);
//    view->showNormal();
    this->setWidget(view);
}

void nitro::NodeView::clearModel() {
    if (dataFlowGraphModel) {
        for (const auto &item: dataFlowGraphModel->allNodeIds()) {
            dataFlowGraphModel->deleteNode(item);
        }
    }

}

nitro::NodeView::~NodeView() = default;
