#pragma once

#include <QString>
#include <QColor>

namespace nitro {

    class NodeInfo {

    public:
        const QString &getNodeName() const {
            return nodeName_;
        }

        const QString &getNodeId() const {
            return nodeId_;
        }

        const QColor &getNodeColor() const {
            return nodeColor_;
        }

        const QString &getIconPath() const {
            return iconPath_;
        }

        NodeInfo(const QString& nodeName, const QString& nodeId, const QColor& nodeColor, const QString& iconPath) : nodeName_(nodeName), nodeId_(nodeId), nodeColor_(nodeColor), iconPath_(iconPath) {

        }

    private:
        QString nodeName_;
        QString nodeId_;
        QColor nodeColor_;
        QString iconPath_;
    };

} // nitro