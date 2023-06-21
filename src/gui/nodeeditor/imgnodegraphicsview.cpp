#include "imgnodegraphicsview.hpp"
#include "QtNodes/DataFlowGraphModel"

#include "src/util/imgresourcereader.hpp"
#include "QtNodes/internal/AbstractNodeGeometry.hpp"

#include "util.hpp"

#include "QtNodes/internal/ConnectionGraphicsObject.hpp"
#include "QtNodes/internal/NodeGraphicsObject.hpp"
#include "QtNodes/InvalidData.hpp"
#include "QtNodes/Definitions"
#include <QAction>
#include "QtNodes/BasicGraphicsScene"
#include "nodes/datatypes/colimagedata.hpp"
#include "QtNodes/internal/UndoCommands.hpp"
#include <QMenu>
#include <QUndoStack>

#include <QKeyEvent>
#include <QLineEdit>
#include <QWidgetAction>


nitro::ImageNodeGraphicsView::ImageNodeGraphicsView(NodeRegistry *nodes,
                                                    QtNodes::BasicGraphicsScene *scene,
                                                    QtNodes::DataFlowGraphModel *model, QWidget *parent)
        : NodeGraphicsView(scene, model, parent),
          nodeBeingViewed_(QtNodes::InvalidNodeId),
          nodeGeometry_(scene->nodeGeometry()),
          nodes_(nodes) {}

QAction *
nitro::ImageNodeGraphicsView::spawnNodeAction(const QtNodes::NodeInfo &info) {
    const QString &menuName = info.getNodeName();
    const QString &nodeType = info.getNodeId();
    const QString &iconPath = info.getIconPath();
    QColor icColor = info.getNodeColor();
    auto *createNodeAction = new QAction(menuName, this);
    QObject::connect(createNodeAction, &QAction::triggered, [this, nodeType]() {
        // Mouse position in scene coordinates.
        QPoint globalPos = mapFromGlobal(QCursor::pos());
        if (rect().contains(globalPos)) {
            QPointF posView = mapToScene(globalPos);
            scene_->undoStack().push(new QtNodes::CreateCommand(scene_, nodeType, posView));
        }
    });
    QIcon icon;
    icon.addPixmap(nitro::ImResourceReader::getPixMap(iconPath, {16, 16}, makeReadable(icColor)));
    createNodeAction->setIcon(icon);
    return createNodeAction;
}

QMenu *nitro::ImageNodeGraphicsView::initNodeMenu() {
    auto *menu = new QMenu(this);
    const int padding = 12;
    menu->setContentsMargins(padding,padding,padding,padding);

    QAction *sectionTitle = menu->addSection("Add");
    QFont font;
    font.setWeight(QFont::Light);
    sectionTitle->setFont(font);

    // TODO
    // Add filterbox to the context menu
//    auto *txtBox = new QLineEdit(menu);
//    txtBox->setPlaceholderText(QStringLiteral("Filter"));
//    txtBox->setClearButtonEnabled(true);
//
//    auto *txtBoxAction = new QWidgetAction(menu);
//    txtBoxAction->setDefaultWidget(txtBox);
//
//    menu->addAction(txtBoxAction);
    menu->addSeparator();

    auto categories = nodes_->getCategories();

    // TODO: treeview
    for (const auto &category: categories) {
        auto *subMenu = new QMenu(category.first);
        auto nodeInfos = category.second;
        for (auto &info: nodeInfos) {
            subMenu->addAction(spawnNodeAction(info));
        }
        menu->addMenu(subMenu);
    }

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


            QtNodes::NodeId viewerNodeId = QtNodes::InvalidNodeId;
            auto allNodes = dataModel_->allNodeIds();
            //TODO extract this magic string
            QString viewerNodeName = "ImageViewer";
            for (auto id: allNodes) {
                if (viewerNodeName == dataModel_->nodeData(id, QtNodes::NodeRole::Type).value<QString>()) {
                    viewerNodeId = id;
                    break;
                }
            }
            if (!dataModel_->nodeExists(viewerNodeId)) {
                // Spawn viewer node
                QtNodes::NodeId const newId = dataModel_->addNode(viewerNodeName);
                viewerNodeId = newId;
                QPointF posView(c->pos().x() + c->boundingRect().width() * 2,
                                c->pos().y() + c->boundingRect().height() / 4);
                dataModel_->setNodeData(viewerNodeId, QtNodes::NodeRole::Position, posView);
            }
            auto viewerType = dataModel_->portData(viewerNodeId, QtNodes::PortType::In, 0,
                                                   QtNodes::PortRole::DataType).value<QtNodes::NodeDataType>();

            auto const &cid = c->nodeId();
            // Find a suitable port to view; multiple clicks will cycle through the ports
            if (nodeBeingViewed_ == cid) {
                int numPorts = dataModel_->nodeData(cid, QtNodes::NodeRole::OutPortCount).toInt();
                for (int i = 0; i < numPorts; ++i) {
                    currentPort_++;
                    auto pData = dataModel_->portData(nodeBeingViewed_, QtNodes::PortType::Out, currentPort_,
                                                      QtNodes::PortRole::DataType).value<QtNodes::NodeDataType>();
                    if (pData.id == viewerType.id || viewerType.allowConversionFrom(pData.id)) {
                        break;
                    }
                    if (pData.id == QtNodes::InvalidData().type().id) {
                        currentPort_ = -1; // will become 0 in the next iteration
                    }
                }
            } else {
                currentPort_ = 0;
            }
            QtNodes::ConnectionId connectionId = {.outNodeId = cid, .outPortIndex = currentPort_, .inNodeId = viewerNodeId, .inPortIndex = 0};
            if (dataModel_->connectionPossible(connectionId)) {

                QtNodes::NodeId const nodeId = getNodeId(QtNodes::PortType::In, connectionId);
                QtNodes::PortIndex const portIndex = getPortIndex(QtNodes::PortType::In, connectionId);
                auto const connections = dataModel_->connections(nodeId, QtNodes::PortType::In, portIndex);
                // Delete existing connections from viewer node
                for (auto &con: connections) {
                    dataModel_->deleteConnection(con);
                }
                dataModel_->addConnection(connectionId);
                nodeBeingViewed_ = cid;
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
