#include "imgnodegraphicsview.hpp"
#include "QtNodes/DataFlowGraphModel"

#include "src/util/imgresourcereader.hpp"
#include "QtNodes/internal/AbstractNodeGeometry.hpp"

#include "include/util.hpp"

#include "QtNodes/internal/ConnectionGraphicsObject.hpp"
#include "QtNodes/internal/NodeGraphicsObject.hpp"
#include "QtNodes/InvalidData.hpp"
#include "QtNodes/Definitions"
#include <QAction>
#include "QtNodes/BasicGraphicsScene"
#include "nodes/datatypes/imagedata.hpp"
#include <QMenu>

#include <QKeyEvent>


nitro::ImageNodeGraphicsView::ImageNodeGraphicsView(NodeRegistry *nodes,
                                                    QtNodes::BasicGraphicsScene *scene,
                                                    QtNodes::DataFlowGraphModel *model, QWidget *parent)
        : NodeGraphicsView(scene, model, parent),
          nodeBeingViewed(QtNodes::InvalidNodeId),
          nodeGeometry(scene->nodeGeometry()),
          nodes_(nodes) {

}

QAction *
nitro::ImageNodeGraphicsView::spawnNodeAction(const QtNodes::NodeInfo &info) {
    const QString &menuName = info.getNodeName();
    const QString &nodeType = info.getNodeId();
    const QString &iconPath = info.getIconPath();
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
    icon.addPixmap(nitro::ImResourceReader::getPixMap(iconPath, {16, 16}, makeReadable(icColor)));
    createNodeAction->setIcon(icon);
    return createNodeAction;
}

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

                //TODO extract this magic string
                QtNodes::NodeId const newId = dataModel_->addNode("ImageViewer");
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
                    if (pData.id == nitro::ImageData().type().id) {
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