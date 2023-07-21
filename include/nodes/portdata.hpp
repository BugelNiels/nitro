#pragma once

#include <utility>

#include "QtNodes/NodeData"

namespace nitro {

class PortData {

public:

    PortData() = default;

    PortData(QString name, const std::shared_ptr<QtNodes::NodeData> &data)
            : name_(std::move(name)),
              type_(data->type()),
              data_(data) {
    }

    PortData(QString name, QtNodes::NodeDataType type)
            : name_(std::move(name)),
              type_(std::move(type)),
              data_(nullptr) {
    }

    [[nodiscard]] const QString &getName() const {
        return name_;
    }

    [[nodiscard]] const QtNodes::NodeDataType &getType() const {
        return type_;
    }

    [[nodiscard]] const std::shared_ptr<QtNodes::NodeData> &getData() const {
        return data_;
    }

private:
    QString name_;
    QtNodes::NodeDataType type_;
    std::shared_ptr<QtNodes::NodeData> data_;

    // TODO
    friend class NodePorts;
};

} // namespace nitro
