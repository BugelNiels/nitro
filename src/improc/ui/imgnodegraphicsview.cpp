#include "imgnodegraphicsview.hpp"
#include <QtNodes/DataFlowGraphModel>

#include "src/improc/datamodels/nitronodes.hpp"

#include "util/imgresourcereader.hpp"
#include <QtNodes/internal/AbstractNodeGeometry.hpp>

#include "util/util.hpp"

#include <QtNodes/internal/ConnectionGraphicsObject.hpp>
#include <QtNodes/internal/NodeGraphicsObject.hpp>
#include "ui/imgviewer/imgviewer.hpp"
#include <QAction>
#include <QtNodes/BasicGraphicsScene>
#include <QMenu>

#include <QKeyEvent>


nitro::ImageNodeGraphicsView::ImageNodeGraphicsView(nitro::ImageViewer *viewer, QtNodes::BasicGraphicsScene *scene,
                                                    QtNodes::DataFlowGraphModel *model, QWidget *parent)
        : NodeGraphicsView(scene, model,
                           parent),
          _imViewer(viewer),
          viewerNodeId(
                  QtNodes::InvalidNodeId),
          nodeIdViewed(
                  QtNodes::InvalidNodeId),
          nodeGeometry(
                  scene->nodeGeometry()) {

}

QAction *
nitro::ImageNodeGraphicsView::spawnNodeAction(const QString &menuName, const QString &nodeType, const QString &iconPath,
                                              const QColor &icColor) {
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

QAction *nitro::ImageNodeGraphicsView::spawnViewerNodeAction() {
    QString menuName = nitro::ImageViewerDataModel::nodeCaption();
    QString nodeType = nitro::ImageViewerDataModel::nodeName();
    QString iconPath = nitro::ImageViewerDataModel::nodeIcon();
    QColor icColor = nitro::ImageViewerDataModel::nodeColor();
    auto *createNodeAction = new QAction(menuName, this);
    QObject::connect(createNodeAction, &QAction::triggered, [this, nodeType]() {
        if (dataModel_->nodeExists(viewerNodeId)) {
            dataModel_->deleteNode(viewerNodeId);
        }
        QPointF posView = this->mapToScene(this->mapFromGlobal(QCursor::pos()));

        QtNodes::NodeId const newId = dataModel_->addNode(nodeType);
        auto rect = nodeGeometry.size(newId);
        posView.setX(posView.x() - rect.width() / 2);
        posView.setY(posView.y() - rect.height() / 2);
        viewerNodeId = newId;
        dataModel_->setNodeData(newId, QtNodes::NodeRole::Position, posView);
    });
    QIcon icon;
    icon.addPixmap(nitro::ImgResourceReader::getPixMap(iconPath, {16, 16}, makeReadable(icColor)));
    createNodeAction->setIcon(icon);
    return createNodeAction;
}

// TODO: check pointer usage
QMenu *nitro::ImageNodeGraphicsView::initInputSubMenu() {
    auto *inputMenu = new QMenu("Input");
    inputMenu->addAction(
            spawnNodeAction(nitro::ImageSourceDataModel::nodeCaption(), nitro::ImageSourceDataModel::nodeName(),
                            nitro::ImageSourceDataModel::nodeIcon(), nitro::ImageSourceDataModel::nodeColor()));
    return inputMenu;
}

QMenu *nitro::ImageNodeGraphicsView::initOutputSubMenu() {
    auto *outputMenu = new QMenu("Output");
    nitro::ImageViewerDataModel::setViewer(_imViewer);
    spawnViewNodeAction = spawnViewerNodeAction();
    outputMenu->addAction(spawnViewNodeAction);
    // TODO: ensure only 1 can be spawned
    outputMenu->addAction(
            spawnNodeAction(nitro::SurfaceViewerDataModel::nodeCaption(), nitro::SurfaceViewerDataModel::nodeName(),
                            nitro::SurfaceViewerDataModel::nodeIcon(), nitro::SurfaceViewerDataModel::nodeColor()));
    return outputMenu;
}

QMenu *nitro::ImageNodeGraphicsView::initColorSubMenu() {
    auto *convertMenu = new QMenu("Color");
    convertMenu->addAction(
            spawnNodeAction(nitro::ThresholdDataModel::nodeCaption(), nitro::ThresholdDataModel::nodeName(),
                            nitro::ThresholdDataModel::nodeIcon(), nitro::ThresholdDataModel::nodeColor()));
    convertMenu->addAction(
            spawnNodeAction(nitro::ToGrayScaleDataModel::nodeCaption(), nitro::ToGrayScaleDataModel::nodeName(),
                            nitro::ToGrayScaleDataModel::nodeIcon(), nitro::ToGrayScaleDataModel::nodeColor()));
    convertMenu->addSeparator();
    convertMenu->addAction(
            spawnNodeAction(nitro::SeparateRgbDataModel::nodeCaption(), nitro::SeparateRgbDataModel::nodeName(),
                            nitro::SeparateRgbDataModel::nodeIcon(), nitro::SeparateRgbDataModel::nodeColor()));
    convertMenu->addAction(
            spawnNodeAction(nitro::CombineRgbDataModel::nodeCaption(), nitro::CombineRgbDataModel::nodeName(),
                            nitro::CombineRgbDataModel::nodeIcon(), nitro::CombineRgbDataModel::nodeColor()));
    convertMenu->addSeparator();
    convertMenu->addAction(
            spawnNodeAction(nitro::SeparateYCbCrDataModel::nodeCaption(), nitro::SeparateYCbCrDataModel::nodeName(),
                            nitro::SeparateYCbCrDataModel::nodeIcon(), nitro::SeparateYCbCrDataModel::nodeColor()));
    convertMenu->addAction(
            spawnNodeAction(nitro::CombineYCbrCrDataModel::nodeCaption(), nitro::CombineYCbrCrDataModel::nodeName(),
                            nitro::CombineYCbrCrDataModel::nodeIcon(), nitro::CombineYCbrCrDataModel::nodeColor()));
    return convertMenu;
}

QMenu *nitro::ImageNodeGraphicsView::initComparisonSubMenu() {
    auto *opsMenu = new QMenu("Comparison");
    opsMenu->addAction(spawnNodeAction(nitro::FlipDataModel::nodeCaption(), nitro::FlipDataModel::nodeName(),
                                       nitro::FlipDataModel::nodeIcon(), nitro::FlipDataModel::nodeColor()));
    return opsMenu;
}

QMenu *nitro::ImageNodeGraphicsView::initMathSubMenu() {
    auto *opsMenu = new QMenu("Util");
    opsMenu->addAction(spawnNodeAction(nitro::ImgMathDataModel::nodeCaption(), nitro::ImgMathDataModel::nodeName(),
                                       nitro::ImgMathDataModel::nodeIcon(), nitro::ImgMathDataModel::nodeColor()));
    opsMenu->addAction(spawnNodeAction(nitro::ToggleDataModel::nodeCaption(), nitro::ToggleDataModel::nodeName(),
                                       nitro::ToggleDataModel::nodeIcon(), nitro::ToggleDataModel::nodeColor()));
    opsMenu->addAction(spawnNodeAction(nitro::BlendDataModel::nodeCaption(), nitro::BlendDataModel::nodeName(),
                                       nitro::BlendDataModel::nodeIcon(), nitro::BlendDataModel::nodeColor()));
    opsMenu->addAction(spawnNodeAction(nitro::MixDataModel::nodeCaption(), nitro::MixDataModel::nodeName(),
                                       nitro::MixDataModel::nodeIcon(), nitro::MixDataModel::nodeColor()));
    // TODO: filter menu
    opsMenu->addAction(spawnNodeAction(nitro::LowPassFilterDataModel::nodeCaption(), nitro::LowPassFilterDataModel::nodeName(),
                                       nitro::LowPassFilterDataModel::nodeIcon(), nitro::LowPassFilterDataModel::nodeColor()));
    return opsMenu;
}

QMenu *nitro::ImageNodeGraphicsView::initQuantizationSubMenu() {
    auto *opsMenu = new QMenu("Quantization");
    opsMenu->addAction(
            spawnNodeAction(nitro::QuantisizeDataModel::nodeCaption(), nitro::QuantisizeDataModel::nodeName(),
                            nitro::QuantisizeDataModel::nodeIcon(), nitro::QuantisizeDataModel::nodeColor()));
    opsMenu->addAction(spawnNodeAction(nitro::KMeansDataModel::nodeCaption(), nitro::KMeansDataModel::nodeName(),
                                       nitro::KMeansDataModel::nodeIcon(), nitro::KMeansDataModel::nodeColor()));
    opsMenu->addAction(spawnNodeAction(nitro::SlrDataModel::nodeCaption(), nitro::SlrDataModel::nodeName(),
                                       nitro::SlrDataModel::nodeIcon(), nitro::SlrDataModel::nodeColor()));
    return opsMenu;
}

QMenu *nitro::ImageNodeGraphicsView::initResampleSubMenu() {
    auto *opsMenu = new QMenu("Resampling");
    opsMenu->addAction(spawnNodeAction(nitro::ResampleDataModel::nodeCaption(), nitro::ResampleDataModel::nodeName(),
                                       nitro::ResampleDataModel::nodeIcon(), nitro::ResampleDataModel::nodeColor()));
    opsMenu->addAction(spawnNodeAction(nitro::LuminanceCorrectionDataModel::nodeCaption(),
                                       nitro::LuminanceCorrectionDataModel::nodeName(),
                                       nitro::LuminanceCorrectionDataModel::nodeIcon(),
                                       nitro::LuminanceCorrectionDataModel::nodeColor()));
    return opsMenu;
}

QMenu *nitro::ImageNodeGraphicsView::initNodeMenu() {
    auto *menu = new QMenu(this);
    auto *inputMenu = initInputSubMenu();
    auto *outputMenu = initOutputSubMenu();
    auto *quantMenu = initQuantizationSubMenu();
    auto *resampleMenu = initResampleSubMenu();
    auto *compMenu = initComparisonSubMenu();
    auto *mathMenu = initMathSubMenu();
    auto *colorMenu = initColorSubMenu();

    QAction *sectionTitle = menu->addSection("Add");
    QFont font;
    font.setWeight(QFont::Light);
    sectionTitle->setFont(font);

    menu->addSeparator();
    menu->addMenu(inputMenu);
    menu->addMenu(outputMenu);
    menu->addMenu(quantMenu);
    menu->addMenu(resampleMenu);
    menu->addMenu(compMenu);
    menu->addMenu(mathMenu);
    menu->addMenu(colorMenu);
    menu->setMaximumSize(menu->sizeHint());
    return menu;
}


void nitro::ImageNodeGraphicsView::mousePressEvent(QMouseEvent *event) {
    if (event->modifiers().testFlag(Qt::ControlModifier) && event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->button() == Qt::LeftButton) {
        // Spawn and connect to viewer if possible
        QGraphicsItem *item = itemAt(event->pos().x(), event->pos().y());
        // Update position of current selected node?
        if (item != nullptr) {

            QtNodes::NodeGraphicsObject *ngo;
            ngo = qgraphicsitem_cast<QtNodes::NodeGraphicsObject *>(item);
            while (!ngo) {
                item = item->parentItem();
                ngo = qgraphicsitem_cast<QtNodes::NodeGraphicsObject *>(item);
            }
            if (auto c = qgraphicsitem_cast<QtNodes::NodeGraphicsObject *>(item)) {

                if (!dataModel_->nodeExists(viewerNodeId)) {
                    // Spawn viewer node
                    QPointF posView(c->pos().x() + c->boundingRect().width() + 5,
                                    c->pos().y() + c->boundingRect().height() / 4);


                    QtNodes::NodeId const newId = dataModel_->addNode(nitro::ImageViewerDataModel::nodeName());
                    viewerNodeId = newId;
                    dataModel_->setNodeData(newId, QtNodes::NodeRole::Position, posView);
                }


                auto const &cid = c->nodeId();
                if (cid == viewerNodeId) {
                    // skip being able to view the viewer itself;
                    return;
                }
                if (nodeIdViewed == cid) {
                    while (true) {
                        currentPort++;
                        auto pData = dataModel_->portData(nodeIdViewed, QtNodes::PortType::Out, currentPort,
                                                          QtNodes::PortRole::DataType).value<QtNodes::NodeDataType>();
                        if (pData.id == nitro::ImageData().type().id) {
                            break;
                        }
                        if (pData.id == nitro::InvalidData().type().id) {
                            currentPort = -1; // will become 0 in the next iteration
                        }
                    }
                } else {
                    nodeIdViewed = cid;
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
    } else {
        QtNodes::GraphicsView::mousePressEvent(event);
    }
}

void nitro::ImageNodeGraphicsView::setViewerNodeId(QtNodes::NodeId nodeId) {
    viewerNodeId = nodeId;
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