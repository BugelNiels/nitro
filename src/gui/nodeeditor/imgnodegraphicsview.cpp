#include "imgnodegraphicsview.hpp"

#include "src/util/imgresourcereader.hpp"
#include <util.hpp>

#include <QtNodes/internal/ConnectionGraphicsObject.hpp>
#include <QtNodes/internal/NodeGraphicsObject.hpp>
#include <QtNodes/InvalidData.hpp>
#include <QtNodes/Definitions>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/internal/UndoCommands.hpp>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/internal/AbstractNodeGeometry.hpp>

#include <QAction>
#include <QMenu>
#include <QMimeData>
#include <QUndoStack>
#include <QApplication>
#include <QKeyEvent>
#include <QLineEdit>

namespace nitro {

ImageNodeGraphicsView::ImageNodeGraphicsView(std::shared_ptr<NodeRegistry>& nodes,
                                             QtNodes::BasicGraphicsScene *scene,
                                             QtNodes::DataFlowGraphModel *model, QWidget *parent)
        : NodeGraphicsView(scene, model, parent),
          nodeBeingViewed_(QtNodes::InvalidNodeId),
          nodes_(nodes) {
    setAcceptDrops(true);
}

QAction *
ImageNodeGraphicsView::spawnNodeAction(const QtNodes::NodeInfo &info) {
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
    icon.addPixmap(ImResourceReader::getPixMap(iconPath, {16, 16}, makeReadable(icColor)));
    createNodeAction->setIcon(icon);
    return createNodeAction;
}

QMenu *ImageNodeGraphicsView::initNodeMenu() {
    auto *menu = new QMenu(this);
    const int padding = 12;
    menu->setContentsMargins(padding, padding, padding, padding);

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


void ImageNodeGraphicsView::spawnViewerNodeAt(int x, int y) {
    // Spawn and connect to viewer if possible
    QGraphicsItem *item = itemAt(x, y);
    // Update position of current selected node?
    if (item != nullptr) {

        QtNodes::NodeGraphicsObject *ngo;
        ngo = qgraphicsitem_cast<QtNodes::NodeGraphicsObject *>(item);
        while (!ngo) {
            if (item == nullptr) {
                return;
            }
            item = item->parentItem();
            ngo = qgraphicsitem_cast<QtNodes::NodeGraphicsObject *>(item);
        }
        if (auto c = qgraphicsitem_cast<QtNodes::NodeGraphicsObject *>(item)) {


            QtNodes::NodeId viewerNodeId = QtNodes::InvalidNodeId;
            auto allNodes = dataModel_->allNodeIds();
            //TODO extract this magic string
            // TODO: Extract global map somewhere that supports all viewers
            // Loop over the ports and check if there is a viewer node available for said port
            QString viewerNodeName = "ImageViewer";
            for (auto id: allNodes) {
                if (viewerNodeName == dataModel_->nodeData(id, QtNodes::NodeRole::Type).value<QString>()) {
                    viewerNodeId = id;
                    break;
                }
            }
            QtNodes::CreateCommand *createCommand = nullptr;
            if (!dataModel_->nodeExists(viewerNodeId)) {
                // Spawn viewer node
                QPointF posView(c->pos().x() + c->boundingRect().width() * 2,
                                c->pos().y() + c->boundingRect().height() / 4);
                createCommand = new QtNodes::CreateCommand(scene_, viewerNodeName, posView);
                scene_->undoStack().push(createCommand);
                viewerNodeId = createCommand->getNodeId();

            }
            auto viewerType = dataModel_->portData(viewerNodeId, QtNodes::PortType::In, 0,
                                                   QtNodes::PortRole::DataType).value<QtNodes::NodeDataType>();

            auto const &cid = c->nodeId();
            if(viewerNodeName == dataModel_->nodeData(cid, QtNodes::NodeRole::Type).value<QString>()) {
                return;
            }
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
            } else if (createCommand != nullptr) {
                createCommand->undo();
            }
        }
    }
}

void ImageNodeGraphicsView::mousePressEvent(QMouseEvent *event) {
    if (event->modifiers().testFlag(Qt::ControlModifier) && event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->button() == Qt::LeftButton) {
        spawnViewerNodeAt(event->pos().x(), event->pos().y());
    } else {
        QtNodes::GraphicsView::mousePressEvent(event);
    }
}

void ImageNodeGraphicsView::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->modifiers().testFlag(Qt::ControlModifier) && event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->button() == Qt::LeftButton) {
        // Fix to ensure we can cycle the viewer node quickly between ports
        mousePressEvent(event);
        return;
    }
    QGraphicsView::mouseDoubleClickEvent(event);
}

void ImageNodeGraphicsView::dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasUrls()) {
        QList<QUrl> urls = event->mimeData()->urls();
        if (!urls.isEmpty()) {
            QString filePath = urls.first().toLocalFile();
            // Process the file path as needed
            QPointF posView = mapToScene(mapFromGlobal(QCursor::pos()));
            //TODO: definitely refactor this; quite hardcoded at the moment
            QString sourceNodeName = "ImageSource";
            auto createCommand = new QtNodes::CreateCommand(scene_, sourceNodeName, posView);
            scene_->undoStack().push(createCommand);
            auto sourceNodeId = createCommand->getNodeId();
            auto json = dataModel_->saveNode(sourceNodeId);
            QJsonObject internalData = json["internal-data"].toObject();
            QJsonObject properties = internalData["properties"].toObject();
            properties["Out 0"] = filePath;
            internalData["properties"] = properties;
            json["internal-data"] = internalData;
            dataModel_->deleteNode(sourceNodeId);
            dataModel_->loadNode(json);

            QApplication::restoreOverrideCursor();
        }
    }
}

void ImageNodeGraphicsView::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();

}

void ImageNodeGraphicsView::dragMoveEvent(QDragMoveEvent *event) {
    QGraphicsView::dragMoveEvent(event);
    event->acceptProposedAction();
}

} // namespace nitro
