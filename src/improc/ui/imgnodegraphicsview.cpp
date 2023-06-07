#include "imgnodegraphicsview.hpp"
#include <QtNodes/DataFlowGraphModel>

#include "src/core/nodes/initialization/nitronodes.hpp"

#include "util/imgresourcereader.hpp"
#include <QtNodes/internal/AbstractNodeGeometry.hpp>

#include "util/util.hpp"

#include <QtNodes/internal/ConnectionGraphicsObject.hpp>
#include <QtNodes/internal/NodeGraphicsObject.hpp>
#include "ui/imgviewer/imgviewer.hpp"
#include "datamodels/output/imageviewerdatamodel.hpp"
#include "3rdparty/nodeeditor/include/QtNodes/InvalidData.hpp"
#include "QtNodes/Definitions"
#include "nodes/datatypes/greyimagedata.hpp"
#include <QAction>
#include <QtNodes/BasicGraphicsScene>
#include <QMenu>

#include <QKeyEvent>


nitro::ImageNodeGraphicsView::ImageNodeGraphicsView(NitroNodes *nodes, nitro::ImageViewer *viewer,
                                                    QtNodes::BasicGraphicsScene *scene,
                                                    QtNodes::DataFlowGraphModel *model, QWidget *parent)
        : NodeGraphicsView(scene, model, parent),
          _imViewer(viewer),
          nodeBeingViewed(QtNodes::InvalidNodeId),
          nodeGeometry(scene->nodeGeometry()),
          nodes_(nodes) {

}

QAction *
nitro::ImageNodeGraphicsView::spawnNodeAction(const QtNodes::NodeInfo &info) {
    QString menuName = info.getNodeName();
    QString nodeType = info.getNodeId();
    QString iconPath = info.getIconPath();
    QColor icColor = info.getNodeColor();
    auto *createNodeAction = new QAction(menuName, this);
    QObject::connect(createNodeAction, &QAction::triggered, [this, nodeType]() {
        // Mouse position in scene coordinates.
        QPointF posView = this->mapToScene(this->mapFromGlobal(QCursor::pos()));

        QtNodes::NodeId const newId = dataModel_->addNode(nodeType);
        auto rect = nodeGeometry.size(newId);
        posView.setX(posView.x() - rect.width() / 2);
        posView.setY(posView.y() - rect.height() / 2);
        dataModel_->setNodeData(newId, QtNodes::NodeRole::Position, posView);
    });
    QIcon icon;
    icon.addPixmap(nitro::ImgResourceReader::getPixMap(iconPath, {16, 16}, makeReadable(icColor)));
    createNodeAction->setIcon(icon);
    return createNodeAction;
}

QAction *nitro::ImageNodeGraphicsView::spawnOutputNodeAction() {
//    nitro::NodeInfo info = nitro::ImageViewerDataModel::nodeInfo();
//    QString menuName = info.getNodeName();
//    QString nodeType = info.getNodeId();
//    QString iconPath = info.getIconPath();
//    QColor icColor = info.getNodeColor();
//    auto *createNodeAction = new QAction(menuName, this);
//    QObject::connect(createNodeAction, &QAction::triggered, [this, nodeType]() {
//        if (dataModel_->nodeExists(viewerNodeId)) {
//            dataModel_->deleteNode(viewerNodeId);
//        }
//        QPointF posView = this->mapToScene(this->mapFromGlobal(QCursor::pos()));
//
//        QtNodes::NodeId const newId = dataModel_->addNode(nodeType);
//        // TODO: bounding box of all
//        auto rect = nodeGeometry.size(newId);
//        posView.setX(posView.x() - rect.width() / 2);
//        posView.setY(posView.y() - rect.height() / 2);
//        viewerNodeId = newId;
//        dataModel_->setNodeData(newId, QtNodes::NodeRole::Position, posView);
//    });
//    QIcon icon;
//    icon.addPixmap(nitro::ImgResourceReader::getPixMap(iconPath, {16, 16}, makeReadable(icColor)));
//    createNodeAction->setIcon(icon);
//    return createNodeAction;
}

//
//// TODO: check pointer usage
//QMenu *nitro::ImageNodeGraphicsView::initInputSubMenu() {
//    auto *inputMenu = new QMenu("Input");
//    inputMenu->addAction(spawnNodeAction(nitro::ImageSourceDataModel::nodeInfo()));
//    inputMenu->addAction(spawnNodeAction(nitro::DecimalSourceDataModel::nodeInfo()));
//    inputMenu->addAction(spawnNodeAction(nitro::IntegerSourceDataModel::nodeInfo()));
//    return inputMenu;
//}
//
//QMenu *nitro::ImageNodeGraphicsView::initOutputSubMenu() {
//    auto *outputMenu = new QMenu("Output");
//    nitro::ImageViewerDataModel::setViewer(_imViewer);
//    spawnViewNodeAction = spawnViewerNodeAction();
//    outputMenu->addAction(spawnViewNodeAction);
//    // TODO: ensure only 1 can be spawned
//    outputMenu->addAction(spawnNodeAction(nitro::SurfaceViewerDataModel::nodeInfo()));
//    return outputMenu;
//}
//
//QMenu *nitro::ImageNodeGraphicsView::initConverterSubMenu() {
//    auto *convertMenu = new QMenu("Converter");
//    convertMenu->addAction(spawnNodeAction(nitro::ImgMathDataModel::nodeInfo()));
//    convertMenu->addAction(spawnNodeAction(nitro::ToGrayScaleDataModel::nodeInfo()));
//    convertMenu->addSeparator();
//    convertMenu->addAction(spawnNodeAction(nitro::SeparateRgbDataModel::nodeInfo()));
//    convertMenu->addAction(spawnNodeAction(nitro::SeparateYCbCrDataModel::nodeInfo()));
//    convertMenu->addAction(spawnNodeAction(nitro::SeparateICtCpDataModel::nodeInfo()));
//    convertMenu->addSeparator();
//    convertMenu->addAction(spawnNodeAction(nitro::CombineRgbDataModel::nodeInfo()));
//    convertMenu->addAction(spawnNodeAction(nitro::CombineYCbrCrDataModel::nodeInfo()));
//    convertMenu->addAction(spawnNodeAction(nitro::CombineICtCpDataModel::nodeInfo()));
//    return convertMenu;
//}
//
//QMenu *nitro::ImageNodeGraphicsView::initComparisonSubMenu() {
//    auto *opsMenu = new QMenu("Comparison");
//    opsMenu->addAction(spawnNodeAction(nitro::FlipDataModel::nodeInfo()));
//    opsMenu->addAction(spawnNodeAction(nitro::ToggleDataModel::nodeInfo()));
//    return opsMenu;
//}
//
//QMenu *nitro::ImageNodeGraphicsView::initFilterSubMenu() {
//    auto *opsMenu = new QMenu("Filter");
//    opsMenu->addAction(spawnNodeAction(nitro::LowPassFilterDataModel::nodeInfo()));
//    opsMenu->addAction(spawnNodeAction(nitro::ThresholdDataModel::nodeInfo()));
//    return opsMenu;
//}
//
//QMenu *nitro::ImageNodeGraphicsView::initColorSubMenu() {
//    auto *opsMenu = new QMenu("Color");
//    opsMenu->addAction(spawnNodeAction(nitro::BlendDataModel::nodeInfo()));
//    opsMenu->addAction(spawnNodeAction(nitro::MixDataModel::nodeInfo()));
//    return opsMenu;
//}
//
//QMenu *nitro::ImageNodeGraphicsView::initQuantizationSubMenu() {
//    auto *opsMenu = new QMenu("Quantization");
//    opsMenu->addAction(
//            spawnNodeAction(nitro::QuantisizeDataModel::nodeInfo()));
//    opsMenu->addAction(spawnNodeAction(nitro::KMeansDataModel::nodeInfo()));
//    opsMenu->addAction(spawnNodeAction(nitro::LayerSelectionDataModel::nodeInfo()));
//    opsMenu->addAction(spawnNodeAction(nitro::SlrDataModel::nodeInfo()));
//    return opsMenu;
//}
//
//QMenu *nitro::ImageNodeGraphicsView::initResampleSubMenu() {
//    auto *opsMenu = new QMenu("Resampling");
//    opsMenu->addAction(spawnNodeAction(nitro::ResampleDataModel::nodeInfo()));
//
//
//    auto *action = new QAction("Threshold action", this);
//
//    QObject::connect(action, &QAction::triggered, [this]() {
//        // Mouse position in scene coordinates.
//        QPointF posView = this->mapToScene(this->mapFromGlobal(QCursor::pos()));
//        QtNodes::NodeId const newId = dataModel_->addNode("t3");
//        auto rect = nodeGeometry.size(newId);
//        posView.setX(posView.x() - rect.width() / 2);
//        posView.setY(posView.y() - rect.height() / 2);
//        dataModel_->setNodeData(newId, QtNodes::NodeRole::Position, posView);
//    });
//    opsMenu->addAction(action);
//
//
//    return opsMenu;
//}

QMenu *nitro::ImageNodeGraphicsView::initNodeMenu() {
    auto *menu = new QMenu(this);

    QAction *sectionTitle = menu->addSection("Add");
    QFont font;
    font.setWeight(QFont::Light);
    sectionTitle->setFont(font);

    menu->addSeparator();

    auto categories = nodes_->getCategories();

    for (const auto &category: categories) {
        auto *subMenu = new QMenu(category.first);
        auto nodeInfos = category.second;
        for (auto &info: nodeInfos) {
            subMenu->addAction(spawnNodeAction(info));
        }
        menu->addMenu(subMenu);
    }



//    menu->addMenu(initInputSubMenu());
//    menu->addMenu(initOutputSubMenu());
//    menu->addMenu(initFilterSubMenu());
//    menu->addMenu(initQuantizationSubMenu());
//    menu->addMenu(initResampleSubMenu());
//    menu->addMenu(initComparisonSubMenu());
//    menu->addMenu(initColorSubMenu());
//    menu->addMenu(initConverterSubMenu());
    menu->setMaximumSize(menu->sizeHint());
    return menu;
}


void nitro::ImageNodeGraphicsView::spawnViewerNodeAt(int x, int y) {
    // Spawn and connect to viewer if possible
    QGraphicsItem *item = itemAt(x, y);
    // Update position of current selected node?
    if (item != nullptr) {

        QtNodes::NodeGraphicsObject *ngo;
        ngo = qgraphicsitem_cast<QtNodes::NodeGraphicsObject *>(item);
        while (!ngo) {
            item = item->parentItem();
            ngo = qgraphicsitem_cast<QtNodes::NodeGraphicsObject *>(item);
        }
        if (auto c = qgraphicsitem_cast<QtNodes::NodeGraphicsObject *>(item)) {


            QtNodes::NodeId viewerNodeId;
            auto allNodes = dataModel_->allNodeIds();
            QString viewerNodeName = "ImageViewer";
            for (auto id: allNodes) {
                if (viewerNodeName == dataModel_->nodeData(id, QtNodes::NodeRole::Type).value<QString>()) {
                    viewerNodeId = id;
                    break;
                }
            }


            if (!dataModel_->nodeExists(viewerNodeId)) {
                // Spawn viewer node
                QPointF posView(c->pos().x() + c->boundingRect().width() + 5,
                                c->pos().y() + c->boundingRect().height() / 4);


                QtNodes::NodeId const newId = dataModel_->addNode(
                        nitro::ImageViewerDataModel::nodeInfo().getNodeId());
                viewerNodeId = newId;
                dataModel_->setNodeData(newId, QtNodes::NodeRole::Position, posView);
            }


            auto const &cid = c->nodeId();
            if (cid == viewerNodeId) {
                // skip being able to view the viewer itself;
                return;
            }
            // Find a suitable port to view; multiple clicks will cycle through the ports
            if (nodeBeingViewed == cid) {
                while (true) {
                    currentPort++;
                    auto pData = dataModel_->portData(nodeBeingViewed, QtNodes::PortType::Out, currentPort,
                                                      QtNodes::PortRole::DataType).value<QtNodes::NodeDataType>();
                    if (pData.id == nitro::GreyImageData().type().id) {
                        break;
                    }
                    if (pData.id == QtNodes::InvalidData().type().id) {
                        currentPort = -1; // will become 0 in the next iteration
                    }
                }
            } else {
                nodeBeingViewed = cid;
                currentPort = 0;
            }
            QtNodes::ConnectionId connectionId = {.outNodeId = cid, .outPortIndex = currentPort, .inNodeId = viewerNodeId, .inPortIndex = 0};

            auto getDataType = [&](QtNodes::PortType const portType) {
                return dataModel_->portData(getNodeId(portType, connectionId), portType,
                                            getPortIndex(portType, connectionId),
                                            QtNodes::PortRole::DataType).value<QtNodes::NodeDataType>();
            };

            // Check if connection possible
            if (getDataType(QtNodes::PortType::Out).id == getDataType(QtNodes::PortType::In).id) {

                QtNodes::NodeId const nodeId = getNodeId(QtNodes::PortType::In, connectionId);
                QtNodes::PortIndex const portIndex = getPortIndex(QtNodes::PortType::In, connectionId);
                auto const connections = dataModel_->connections(nodeId, QtNodes::PortType::In, portIndex);
                // Delete existing connections from viewer node
                for (auto &con: connections) {
                    _imViewer->awaitReplacement();
                    dataModel_->deleteConnection(con);
                }
                dataModel_->addConnection(connectionId);
            }
        }
    }
}

void nitro::ImageNodeGraphicsView::mousePressEvent(QMouseEvent *event) {
    if (event->modifiers().testFlag(Qt::ControlModifier) && event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->button() == Qt::LeftButton) {
        spawnViewerNodeAt(event->pos().x(), event->pos().y());
    } else {
        QtNodes::GraphicsView::mousePressEvent(event);
    }
}

void nitro::ImageNodeGraphicsView::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->modifiers().testFlag(Qt::ControlModifier) && event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->button() == Qt::LeftButton) {
        // Fix to ensure we can cycle the viewer node quickly between ports
        mousePressEvent(event);
        return;
    }
    QGraphicsView::mouseDoubleClickEvent(event);
}