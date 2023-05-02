#include "nodegraphicsview.hpp"
#include "QtNodes/DataFlowGraphModel"
#include "src/nodes/input/imagesourcedatamodel.hpp"
#include "src/nodes/output/imageviewerdatamodel.hpp"
#include "src/nodes/conversions/tograyscaledatamodel.hpp"
#include "src/nodes/operators/thresholddatamodel.hpp"
#include "src/nodes/operators/quantization/kmeansdatamodel.hpp"
#include "src/nodes/operators/quantization/quantisizedatamodel.hpp"
#include "src/nodes/operators/flipdatamodel.hpp"
#include "src/nodes/operators/reconstruction/resampledatamodel.hpp"
#include "src/nodes/conversions/seperatergbdatamodel.hpp"
#include "src/nodes/invaliddata.hpp"
#include "util/imgresourcereader.hpp"

#include <QtNodes/internal/ConnectionGraphicsObject.hpp>
#include <QtNodes/internal/NodeGraphicsObject.hpp>
#include <QAction>
#include <QMenu>

#include <QKeyEvent>


nitro::NodeGraphicsView::NodeGraphicsView(nitro::ImageViewer *viewer, QtNodes::BasicGraphicsScene *scene,
                                          QtNodes::DataFlowGraphModel *model,
                                          QWidget *parent) : GraphicsView(scene,
                                                                          parent), _dataModel(model),
                                                             _imViewer(viewer), viewerNodeId(QtNodes::InvalidNodeId),
                                                             nodeIdViewed(QtNodes::InvalidNodeId) {
    auto *spawnMenu = new QAction(QStringLiteral("Add node"), this);
    spawnMenu->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
    spawnMenu->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_A));
    connect(spawnMenu,
            &QAction::triggered,
            this,
            &NodeGraphicsView::spawnNodeMenu);
    insertAction(actions().front(), spawnMenu);
    _nodeMenu = initNodeMenu();
    setScaleRange(0.3, 1);
}

QAction *nitro::NodeGraphicsView::spawnNodeAction(const QString &menuName, const QString &nodeType, const QString &iconPath) {
    auto *createNodeAction = new QAction(menuName, this);
    QObject::connect(createNodeAction, &QAction::triggered, [this, nodeType]() {
        // Mouse position in scene coordinates.
        QPointF posView = this->mapToScene(this->mapFromGlobal(QCursor::pos()));
        // TODO: get node size and offset

        QtNodes::NodeId const newId = _dataModel->addNode(nodeType);
        _dataModel->setNodeData(newId, QtNodes::NodeRole::Position, posView);
    });
    QIcon icon;
    icon.addPixmap(nitro::ImgResourceReader::getPixMap(iconPath));
    createNodeAction->setIcon(icon);
    return createNodeAction;
}

QAction *nitro::NodeGraphicsView::spawnViewerNodeAction() {
    QString menuName = nitro::ImageViewerDataModel::nodeCaption();
    QString nodeType = nitro::ImageViewerDataModel::nodeName();
    QString iconPath = nitro::ImageViewerDataModel::nodeIcon();
    auto *createNodeAction = new QAction(menuName, this);
    QObject::connect(createNodeAction, &QAction::triggered, [this, nodeType]() {
        if (_dataModel->nodeExists(viewerNodeId)) {
            _dataModel->deleteNode(viewerNodeId);
//            return;
        }
        QPointF posView = this->mapToScene(this->mapFromGlobal(QCursor::pos()));

        QtNodes::NodeId const newId = _dataModel->addNode(nodeType);
        viewerNodeId = newId;
        _dataModel->setNodeData(newId, QtNodes::NodeRole::Position, posView);
    });
    QIcon icon;
    icon.addPixmap(nitro::ImgResourceReader::getPixMap(iconPath));
    createNodeAction->setIcon(icon);
    return createNodeAction;
}

// TODO: check pointer usage
QMenu *nitro::NodeGraphicsView::initInputSubMenu() {
    auto *inputMenu = new QMenu("Input");
    inputMenu->addAction(spawnNodeAction(nitro::ImageSourceDataModel::nodeCaption(),
                                         nitro::ImageSourceDataModel::nodeName(), nitro::ImageSourceDataModel::nodeIcon()));
    return inputMenu;
}

QMenu *nitro::NodeGraphicsView::initOutputSubMenu() {
    auto *outputMenu = new QMenu("Output");
    nitro::ImageViewerDataModel::setViewer(_imViewer);
    spawnViewNodeAction = spawnViewerNodeAction();
    outputMenu->addAction(spawnViewNodeAction);
    return outputMenu;
}

QMenu *nitro::NodeGraphicsView::initColorSubMenu() {
    auto *convertMenu = new QMenu("Color");
    convertMenu->addAction(spawnNodeAction(nitro::ToGrayScaleDataModel::nodeCaption(),
                                           nitro::ToGrayScaleDataModel::nodeName(), nitro::ToGrayScaleDataModel::nodeIcon()));
    convertMenu->addAction(spawnNodeAction(nitro::SeperateRgbDataModel::nodeCaption(),
                                           nitro::SeperateRgbDataModel::nodeName(), nitro::SeperateRgbDataModel::nodeIcon()));
    return convertMenu;
}

QMenu *nitro::NodeGraphicsView::initOperationsSubMenu() {
    auto *opsMenu = new QMenu("Comparison");
    opsMenu->addAction(spawnNodeAction(nitro::FlipDataModel::nodeCaption(),
                                       nitro::FlipDataModel::nodeName(), nitro::FlipDataModel::nodeIcon()));
    return opsMenu;
}

QMenu *nitro::NodeGraphicsView::initQuantizationSubMenu() {
    auto *opsMenu = new QMenu("Quantization");
    opsMenu->addAction(spawnNodeAction(nitro::QuantisizeDataModel::nodeCaption(),
                                       nitro::QuantisizeDataModel::nodeName(), nitro::QuantisizeDataModel::nodeIcon()));
    opsMenu->addAction(spawnNodeAction(nitro::KMeansDataModel::nodeCaption(),
                                       nitro::KMeansDataModel::nodeName(), nitro::KMeansDataModel::nodeIcon()));
    return opsMenu;
}

QMenu *nitro::NodeGraphicsView::initResampleSubMenu() {
    auto *opsMenu = new QMenu("Resampling");
    opsMenu->addAction(spawnNodeAction(nitro::ResampleDataModel::nodeCaption(),
                                       nitro::ResampleDataModel::nodeName(), nitro::ResampleDataModel::nodeIcon()));
    return opsMenu;
}

QMenu *nitro::NodeGraphicsView::initNodeMenu() {
    auto *menu = new QMenu(this);
    auto *inputMenu = initInputSubMenu();
    auto *outputMenu = initOutputSubMenu();
    auto *quantMenu = initQuantizationSubMenu();
    auto *resampleMenu = initResampleSubMenu();
    auto *opsMenu = initOperationsSubMenu();
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
    menu->addMenu(opsMenu);
    menu->addMenu(colorMenu);
    menu->setMaximumSize(menu->sizeHint());
    return menu;
}

void nitro::NodeGraphicsView::spawnNodeMenu() {
    _nodeMenu->popup(QCursor::pos());
}


void nitro::NodeGraphicsView::mousePressEvent(QMouseEvent *event) {
    // TODO: disable control deselecting things
    // Disable the mouse press event from arriving
    qDebug() << "Registered click";
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

                if (!_dataModel->nodeExists(viewerNodeId)) {
                    // Spawn viewer node
                    QPointF posView(c->pos().x() + c->boundingRect().width() + 5,
                                    c->pos().y() + c->boundingRect().height() / 4);


                    QtNodes::NodeId const newId = _dataModel->addNode(nitro::ImageViewerDataModel::nodeName());
                    viewerNodeId = newId;
                    _dataModel->setNodeData(newId, QtNodes::NodeRole::Position, posView);
                }


                auto const &cid = c->nodeId();
                if (cid == viewerNodeId) {
                    // skip being able to view the viewer itself;
                    return;
                }
                if (nodeIdViewed == cid) {
                    while (true) {
                        currentPort++;
                        auto pData = _dataModel->portData(nodeIdViewed,
                                                          QtNodes::PortType::Out,
                                                          currentPort,
                                                          QtNodes::PortRole::DataType)
                                .value<QtNodes::NodeDataType>();
                        if (pData.id == nitro::ImageData().type().id) {
                            break;
                        }
                        if (pData.id == nitro::InvalidData().type().id) {
                            currentPort = -1; // will become 0 in the next iteration
                        }
                    }
                    qDebug() << "Incremented" << currentPort;
                } else {
                    qDebug() << "Reset";
                    nodeIdViewed = cid;
                    currentPort = 0;
                }
                QtNodes::ConnectionId connectionId = {
                        .outNodeId = cid,
                        .outPortIndex = currentPort,
                        .inNodeId = viewerNodeId,
                        .inPortIndex = 0
                };

                auto getDataType = [&](QtNodes::PortType const portType) {
                    return _dataModel->portData(getNodeId(portType, connectionId),
                                                portType,
                                                getPortIndex(portType, connectionId),
                                                QtNodes::PortRole::DataType)
                            .value<QtNodes::NodeDataType>();
                };

                // Check if connection possible
                if (getDataType(QtNodes::PortType::Out).id == getDataType(QtNodes::PortType::In).id) {

                    QtNodes::NodeId const nodeId = getNodeId(QtNodes::PortType::In, connectionId);
                    QtNodes::PortIndex const portIndex = getPortIndex(QtNodes::PortType::In, connectionId);
                    auto const connections = _dataModel->connections(nodeId, QtNodes::PortType::In, portIndex);
                    // Delete existing connections from viewer node
                    for (auto &con: connections) {
                        _imViewer->awaitReplacement();
                        _dataModel->deleteConnection(con);
                    }
                    _dataModel->addConnection(connectionId);
                }
            }
        }
    } else {
        QtNodes::GraphicsView::mousePressEvent(event);
    }
}

void nitro::NodeGraphicsView::setViewerNodeId(QtNodes::NodeId nodeId) {
    viewerNodeId = nodeId;
}

void nitro::NodeGraphicsView::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->modifiers().testFlag(Qt::ControlModifier) && event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->button() == Qt::LeftButton) {
        // Fix to ensure we can cycle the viewer node quickly between ports
        mousePressEvent(event);
        return;
    }
    QGraphicsView::mouseDoubleClickEvent(event);
}
