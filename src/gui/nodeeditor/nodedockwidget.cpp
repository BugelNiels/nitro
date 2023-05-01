#include "nodedockwidget.hpp"
#include "src/nodes/input/imagesourcedatamodel.hpp"
#include "src/nodes/conversions/tograyscaledatamodel.hpp"
#include "src/nodes/operators/thresholddatamodel.hpp"
#include "nodegraphicsview.hpp"
#include "src/nodes/output/imageviewerdatamodel.hpp"
#include "src/nodes/operators/quantization/kmeansdatamodel.hpp"
#include "src/nodes/operators/quantization/quantisizedatamodel.hpp"
#include "src/nodes/operators/flipdatamodel.hpp"

#include <QtGui/QScreen>
#include <QtNodes/BasicGraphicsScene>
#include <QtWidgets/QApplication>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/NodeDelegateModelRegistry>
#include <QFileDialog>
#include <QMessageBox>


static std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels() {
    auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
    ret->registerModel<nitro::ImageSourceDataModel>("Input");
    ret->registerModel<nitro::ImageViewerDataModel>("Output");
    ret->registerModel<nitro::ToGrayScaleDataModel>("Converter");
    ret->registerModel<nitro::ThresholdDataModel>("Operator");
    ret->registerModel<nitro::KMeansDataModel>("Operator");
    ret->registerModel<nitro::QuantisizeDataModel>("Operator");
    ret->registerModel<nitro::FlipDataModel>("Operator");

    return ret;
}

nitro::NodeDockWidget::NodeDockWidget(nitro::ImageViewer *imViewer, QWidget *parent) : QDockWidget(parent),
                                                                                       filename("untitled.json") {
    setWindowTitle("Node Editor");
    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry = registerDataModels();
    dataFlowGraphModel = new QtNodes::DataFlowGraphModel(registry);
    dataFlowGraphModel->addNode(nitro::ImageSourceDataModel::nodeName());
    auto scene = new QtNodes::BasicGraphicsScene(*dataFlowGraphModel);
    view = new nitro::NodeGraphicsView(imViewer, scene, dataFlowGraphModel, this);
    view->setContextMenuPolicy(Qt::ActionsContextMenu);
    prevSave = dataFlowGraphModel->save();
    this->setWidget(view);
}


nitro::NodeDockWidget::~NodeDockWidget() = default;

void nitro::NodeDockWidget::clearModel() {
    if (dataFlowGraphModel) {
        for (const auto &item: dataFlowGraphModel->allNodeIds()) {
            dataFlowGraphModel->deleteNode(item);
        }
    }
    prevSave = dataFlowGraphModel->save();
}

bool nitro::NodeDockWidget::canQuitSafely() {
    if (prevSave == dataFlowGraphModel->save()) {
        return true;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save changes before closing?", filename,
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes) {
        saveModel();
    } else if (reply == QMessageBox::Cancel) {
        return false;
    }
    return true;
}

void nitro::NodeDockWidget::saveModel(bool askFile) {

    if (!dataFlowGraphModel) {
        return;
    }
    QString filePath;
    if (askFile || filename == "untitled.json") {
        filePath = QFileDialog::getSaveFileName(
                this, "Save NITRO Config", "../saves/" + filename,
                tr("Json Files (*.json)"));
        if (filePath == "") {
            return;
        }
        saveFilePath = filePath;
        filename = QFileInfo(filePath).fileName();
    } else {
        filePath = saveFilePath;
    }
    auto saveObject = dataFlowGraphModel->save();
    QFile jsonFile(filePath);
    if (jsonFile.open(QFile::WriteOnly)) {
        QJsonDocument document;
        document.setObject(saveObject);
        jsonFile.write(document.toJson());
        prevSave = saveObject;
    } else {
        QMessageBox::warning(this, "Failed to open file", filePath);
    }
}

void nitro::NodeDockWidget::forwardKeyPress(QKeyEvent *event) {
    if (!view->hasFocus()) {
        view->keyPressEvent(event);
    }
}

void nitro::NodeDockWidget::loadModel() {
    if (!dataFlowGraphModel) {
        return;
    }
    if (!canQuitSafely()) {
        return;
    }
    QString filePath = QFileDialog::getOpenFileName(
            nullptr, "Load NITRO Config", "../saves/",
            tr("Json Files (*.json)"));
    if (filePath == "") {
        return;
    }
    QFile jsonFile(filePath);
    jsonFile.open(QFile::ReadOnly);
    auto doc = QJsonDocument::fromJson(jsonFile.readAll());
    dataFlowGraphModel->load(doc.object());
    // Ensure we cannot create a second viewer
    for (auto &c: dataFlowGraphModel->allNodeIds()) {
        auto attempt = dataFlowGraphModel->delegateModel<nitro::ImageViewerDataModel>(c);
        if (attempt) {
            view->setViewerNodeId(c);
            break;
        }
    }
}

