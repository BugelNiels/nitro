#include "nitronodepainter.hpp"

#include <cmath>

#include <QtNodes/internal/AbstractGraphModel.hpp>
#include <QtNodes/internal/AbstractNodeGeometry.hpp>
#include <QtNodes/internal/BasicGraphicsScene.hpp>
#include <QtNodes/internal/ConnectionGraphicsObject.hpp>
#include <QtNodes/internal/ConnectionIdUtils.hpp>
#include <QtNodes/internal/NodeGraphicsObject.hpp>
#include <QtNodes/internal/NodeState.hpp>
#include <QtNodes/internal/StyleCollection.hpp>

#include "src/gui/nodeeditor/nodecolors.hpp"


nitro::NitroNodePainter::NitroNodePainter() : QtNodes::AbstractNodePainter() {
    nitro::NodeColors::initColors();
}

void nitro::NitroNodePainter::paint(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const {

    drawNodeBackground(painter, ngo);
    drawNodeCaption(painter, ngo);
    drawNodeHighlight(painter, ngo);

    drawConnectionPoints(painter, ngo);

    drawFilledConnectionPoints(painter, ngo);


    drawEntryLabels(painter, ngo);

    drawResizeRect(painter, ngo);
}

void nitro::NitroNodePainter::drawNodeBackground(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const {
    QtNodes::AbstractGraphModel &model = ngo.graphModel();

    QtNodes::NodeId const nodeId = ngo.nodeId();

    QtNodes::AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();
    QSize size = geometry.size(nodeId);

    QJsonDocument json = QJsonDocument::fromVariant(model.nodeData(nodeId, QtNodes::NodeRole::Style));

    QtNodes::NodeStyle nodeStyle(json.object());

    QPen p(nodeStyle.NormalBoundaryColor, nodeStyle.PenWidth);
    painter->setPen(p);

    QLinearGradient gradient(QPointF(0.0, 0.0), QPointF(0, size.height()));

    gradient.setColorAt(0.0, nodeStyle.GradientColor0);
    gradient.setColorAt(0.10, nodeStyle.GradientColor1);
    gradient.setColorAt(0.90, nodeStyle.GradientColor2);
    gradient.setColorAt(1.0, nodeStyle.GradientColor3);

    painter->setBrush(gradient);

    QRectF boundary(0, 0, size.width(), size.height());

    double const radius = 3.0;
    painter->drawRoundedRect(boundary, radius, radius);
}

void nitro::NitroNodePainter::drawNodeHighlight(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const {
    if (!ngo.isSelected()) {
        return;
    }
    QtNodes::AbstractGraphModel &model = ngo.graphModel();

    QtNodes::NodeId const nodeId = ngo.nodeId();

    QtNodes::AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();

    QSize size = geometry.size(nodeId);

    QJsonDocument json = QJsonDocument::fromVariant(model.nodeData(nodeId, QtNodes::NodeRole::Style));

    QtNodes::NodeStyle nodeStyle(json.object());

    QRectF boundary(0, 0, size.width(), size.height());

    double const radius = 3.0;
    QBrush brush(Qt::transparent);
    painter->setBrush(brush);
    painter->drawRoundedRect(boundary, radius, radius);
}

void nitro::NitroNodePainter::drawConnectionPoints(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const {
    QtNodes::AbstractGraphModel &model = ngo.graphModel();
    QtNodes::NodeId const nodeId = ngo.nodeId();
    QtNodes::AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();

    QJsonDocument json = QJsonDocument::fromVariant(model.nodeData(nodeId, QtNodes::NodeRole::Style));
    QtNodes::NodeStyle nodeStyle(json.object());

    auto const &connectionStyle = QtNodes::StyleCollection::connectionStyle();

    float diameter = nodeStyle.ConnectionPointDiameter;
    auto reducedDiameter = diameter * 0.6;

    for (QtNodes::PortType portType: {QtNodes::PortType::Out, QtNodes::PortType::In}) {
        size_t const n = model
                .nodeData(nodeId,
                          (portType == QtNodes::PortType::Out) ? QtNodes::NodeRole::OutPortCount
                                                               : QtNodes::NodeRole::InPortCount)
                .toUInt();

        for (QtNodes::PortIndex portIndex = 0; portIndex < n; ++portIndex) {
            QPointF p = geometry.portPosition(nodeId, portType, portIndex);

            auto const &dataType = model.portData(nodeId, portType, portIndex, QtNodes::PortRole::DataType)
                    .value<QtNodes::NodeDataType>();

            double r = 1.0;

            QtNodes::NodeState const &state = ngo.nodeState();

            if (auto const *cgo = state.connectionForReaction()) {
                QtNodes::PortType requiredPort = cgo->connectionState().requiredPort();

                if (requiredPort == portType) {
                    QtNodes::ConnectionId possibleConnectionId = makeCompleteConnectionId(cgo->connectionId(),
                                                                                          nodeId,
                                                                                          portIndex);

                    bool const possible = model.connectionPossible(possibleConnectionId);

                    auto cp = cgo->sceneTransform().map(cgo->endPoint(requiredPort));
                    cp = ngo.sceneTransform().inverted().map(cp);

                    auto diff = cp - p;
                    double dist = std::sqrt(QPointF::dotProduct(diff, diff));

                    if (possible) {
                        double const thres = 40.0;
                        r = (dist < thres) ? (2.0 - dist / thres) : 1.0;
                    } else {
                        double const thres = 80.0;
                        r = (dist < thres) ? (dist / thres) : 1.0;
                    }
                }
            }

            if (connectionStyle.useDataDefinedColors()) {
                // Set color based on data type
                painter->setBrush(connectionStyle.normalColor(dataType.id));
            } else {
                painter->setBrush(nodeStyle.ConnectionPointColor);
            }

            painter->drawEllipse(p, reducedDiameter * r, reducedDiameter * r);
        }
    }

    if (ngo.nodeState().connectionForReaction()) {
        ngo.nodeState().resetConnectionForReaction();
    }
}

void nitro::NitroNodePainter::drawFilledConnectionPoints(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const {
    QtNodes::AbstractGraphModel &model = ngo.graphModel();
    QtNodes::NodeId const nodeId = ngo.nodeId();
    QtNodes::AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();

    QJsonDocument json = QJsonDocument::fromVariant(model.nodeData(nodeId, QtNodes::NodeRole::Style));
    QtNodes::NodeStyle nodeStyle(json.object());

    auto diameter = nodeStyle.ConnectionPointDiameter;

    for (QtNodes::PortType portType: {QtNodes::PortType::Out, QtNodes::PortType::In}) {
        size_t const n = model
                .nodeData(nodeId,
                          (portType == QtNodes::PortType::Out) ? QtNodes::NodeRole::OutPortCount
                                                               : QtNodes::NodeRole::InPortCount)
                .toUInt();

        for (QtNodes::PortIndex portIndex = 0; portIndex < n; ++portIndex) {
            QPointF p = geometry.portPosition(nodeId, portType, portIndex);

            auto const &connected = model.connections(nodeId, portType, portIndex);

            if (!connected.empty()) {
                auto const &dataType = model
                        .portData(nodeId, portType, portIndex, QtNodes::PortRole::DataType)
                        .value<QtNodes::NodeDataType>();

                auto const &connectionStyle = QtNodes::StyleCollection::connectionStyle();
                if (connectionStyle.useDataDefinedColors()) {
                    QColor const c = connectionStyle.normalColor(dataType.id);
                    painter->setPen(c);
                    painter->setBrush(c);
                } else {
                    painter->setPen(nodeStyle.FilledConnectionPointColor);
                    painter->setBrush(nodeStyle.FilledConnectionPointColor);
                }

                painter->drawEllipse(p, diameter * 0.4, diameter * 0.4);
            }
        }
    }
}

void nitro::NitroNodePainter::drawNodeCaption(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const {
    QtNodes::AbstractGraphModel &model = ngo.graphModel();
    QtNodes::NodeId const nodeId = ngo.nodeId();
    QtNodes::AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();

    if (!model.nodeData(nodeId, QtNodes::NodeRole::CaptionVisible).toBool())
        return;

    QString const name = model.nodeData(nodeId, QtNodes::NodeRole::Caption).toString();

    QFont f = painter->font();
    f.setBold(true);

    QPointF position = geometry.captionPosition(nodeId);
    position.setX(10);
    position.setY(15);

    QJsonDocument json = QJsonDocument::fromVariant(model.nodeData(nodeId, QtNodes::NodeRole::Style));
    QtNodes::NodeStyle nodeStyle(json.object());


    QSize size = geometry.size(nodeId);
    QRectF boundary(0, 0, size.width(), 20);
    QColor captionCol = nitro::NodeColors::getColor(model.nodeData(nodeId, QtNodes::NodeRole::Type).value<QString>());
    painter->setPen(captionCol);
    double const radius = 3.0;
    painter->setBrush(QBrush(captionCol));
    painter->drawRoundedRect(boundary, radius, radius);

    painter->setFont(f);
    painter->setPen(nodeStyle.FontColor);
    painter->drawText(position, name);


    f.setBold(false);
    painter->setFont(f);
}

void nitro::NitroNodePainter::drawEntryLabels(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const {
    QtNodes::AbstractGraphModel &model = ngo.graphModel();
    QtNodes::NodeId const nodeId = ngo.nodeId();
    QtNodes::AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();

    QJsonDocument json = QJsonDocument::fromVariant(model.nodeData(nodeId, QtNodes::NodeRole::Style));
    QtNodes::NodeStyle nodeStyle(json.object());

    for (QtNodes::PortType portType: {QtNodes::PortType::Out, QtNodes::PortType::In}) {
        unsigned int n = model.nodeData<unsigned int>(nodeId,
                                                      (portType == QtNodes::PortType::Out)
                                                      ? QtNodes::NodeRole::OutPortCount
                                                      : QtNodes::NodeRole::InPortCount);

        for (QtNodes::PortIndex portIndex = 0; portIndex < n; ++portIndex) {
            auto const &connected = model.connections(nodeId, portType, portIndex);

            QPointF p = geometry.portTextPosition(nodeId, portType, portIndex);

            if (connected.empty())
                painter->setPen(nodeStyle.FontColorFaded);
            else
                painter->setPen(nodeStyle.FontColor);

            QString s;

            // TODO: draw custom names here
            if (model.portData<bool>(nodeId, portType, portIndex, QtNodes::PortRole::CaptionVisible)) {
                s = model.portData<QString>(nodeId, portType, portIndex, QtNodes::PortRole::Caption);
            } else {
                auto portData = model.portData(nodeId, portType, portIndex, QtNodes::PortRole::DataType);

                s = portData.value<QtNodes::NodeDataType>().name;
            }

            painter->drawText(p, s);
        }
    }
}

void nitro::NitroNodePainter::drawResizeRect(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const {
    QtNodes::AbstractGraphModel &model = ngo.graphModel();
    QtNodes::NodeId const nodeId = ngo.nodeId();
    QtNodes::AbstractNodeGeometry &geometry = ngo.nodeScene()->nodeGeometry();

    if (model.nodeFlags(nodeId) & QtNodes::NodeFlag::Resizable) {
        painter->setBrush(Qt::gray);

        painter->drawEllipse(geometry.resizeHandleRect(nodeId));
    }
}