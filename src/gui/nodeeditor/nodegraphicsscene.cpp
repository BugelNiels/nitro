#include "nodegraphicsscene.hpp"
#include "nitronodepainter.hpp"
#include "src/nodes/invaliddata.hpp"

#include <QtNodes/NodeDelegateModel>
#include <QKeyEvent>
#include <QJsonArray>
#include <QGraphicsItem>
#include <QtNodes/internal/NodeGraphicsObject.hpp>
#include <QtNodes/ConnectionIdUtils>

nitro::NodeGraphicsScene::NodeGraphicsScene(QtNodes::AbstractGraphModel &graphModel, QObject *parent)
        : BasicGraphicsScene(graphModel, parent), m_graphModel(graphModel) {
    setNodePainter(std::make_unique<NitroNodePainter>(NitroNodePainter()));

}

void nitro::NodeGraphicsScene::keyPressEvent(QKeyEvent *event) {
    QGraphicsScene::keyPressEvent(event);
    if (event->key() == Qt::Key_F) {
        // TODO: clean up big time
        auto sItems = selectedItems();
        if (sItems.size() == 2) {
            // Only fuse when two items are selected
            QtNodes::NodeId id1, id2;
            if (auto n = qgraphicsitem_cast<QtNodes::NodeGraphicsObject *>(sItems[0])) {
                id1 = n->nodeId();
            } else {
                return;
            }
            if (auto n = qgraphicsitem_cast<QtNodes::NodeGraphicsObject *>(sItems[1])) {
                id2 = n->nodeId();
            } else {
                return;
            }

            auto portVacant = [&](QtNodes::NodeId nodeId, QtNodes::PortIndex const portIndex,
                                  QtNodes::PortType const portType) {
                auto const connected = m_graphModel.connections(nodeId, portType, portIndex);

                auto policy = m_graphModel.portData(nodeId, portType, portIndex,
                                                    QtNodes::PortRole::ConnectionPolicyRole)
                        .value<QtNodes::ConnectionPolicy>();

                return connected.empty() || (policy == QtNodes::ConnectionPolicy::Many);
            };

            auto getDataType = [&](QtNodes::NodeId nodeId, QtNodes::PortIndex const portIndex,
                                   QtNodes::PortType const portType) {
                return m_graphModel.portData(nodeId, portType,
                                             portIndex,
                                             QtNodes::PortRole::DataType).value<QtNodes::NodeDataType>();
            };

            // Find free port for node 1
            QtNodes::PortIndex p1 = 0;
            while (!portVacant(id1, p1, QtNodes::PortType::Out)) {
                p1++;
                if (getDataType(id1, p1, QtNodes::PortType::Out).id == nitro::InvalidData().type().id) {
                    return; // No port available
                }
            }
            QtNodes::NodeDataType p1Type = getDataType(id1, p1, QtNodes::PortType::Out);


            // Find free port for node 2
            // Also ensure the data types match
            QtNodes::PortIndex p2 = 0;
            while (!portVacant(id2, p2, QtNodes::PortType::In) &&
                   getDataType(id2, p2, QtNodes::PortType::In).id != p1Type.id) {
                p2++;
                if (getDataType(id2, p2, QtNodes::PortType::In).id == nitro::InvalidData().type().id) {
                    return; // No port available
                }
            }

            QtNodes::ConnectionId connectionId = {.outNodeId = id1, .outPortIndex = p1, .inNodeId = id2, .inPortIndex = p1};

            auto getDataTypeFromPort = [&](QtNodes::PortType const portType) {
                return m_graphModel.portData(getNodeId(portType, connectionId), portType,
                                             getPortIndex(portType, connectionId),
                                             QtNodes::PortRole::DataType).value<QtNodes::NodeDataType>();
            };

            // Check if connection possible
            if (getDataTypeFromPort(QtNodes::PortType::Out).id == getDataTypeFromPort(QtNodes::PortType::In).id) {

                QtNodes::NodeId const nodeId = getNodeId(QtNodes::PortType::In, connectionId);
                QtNodes::PortIndex const portIndex = getPortIndex(QtNodes::PortType::In, connectionId);
                auto const connections = m_graphModel.connections(nodeId, QtNodes::PortType::In, portIndex);

                m_graphModel.addConnection(connectionId);
            }

        }
    }
}
