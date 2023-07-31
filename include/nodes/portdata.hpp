#pragma once

#include <utility>

#include <QtNodes/NodeData>

namespace nitro {

/**
 * @brief Represents data of a single port belonging to a node. It contains information about the name of the port, the data type that this port has and the data itself that is at this port.
 * The name of a port is unique amongst its category. That is, all input ports have unique names and all unique output ports have unique names. However, an output port can have a name that already exists in the input ports.
 */
class PortData {

public:
    /**
     * @brief Creates an empty port data class.
     */
    PortData() = default;

    /**
     * @brief Creates new port data with the given name and data.
     * @param name The name of the port. Note that this name should not already exist.
     * @param data The data that this port contains.
     */
    PortData(QString name, const std::shared_ptr<QtNodes::NodeData> &data)
        : name_(std::move(name)),
          type_(data->type()),
          data_(data) {}

    /**
     * @brief Creates new port data with the given name and data type. The data itself is initialised to be empty.
     * @param name The name of the port. Note that this name should not already exist.
     * @param type The data type this port represents.
     */
    PortData(QString name, QtNodes::NodeDataType type)
        : name_(std::move(name)),
          type_(std::move(type)),
          data_(nullptr) {}

    /**
     * @brief Retrieves the name of the port.
     * @return The name of the port.
     */
    [[nodiscard]] const QString &getName() const { return name_; }

    /**
     * @brief Retrieves the data type of the port.
     * @return The data type of the port.
     */
    [[nodiscard]] const QtNodes::NodeDataType &getType() const { return type_; }

    /**
     * @brief Retrieves the data of the port. Note that this can be a nullptr if it is empty.
     * @return The data of the port.
     */
    [[nodiscard]] const std::shared_ptr<QtNodes::NodeData> &getData() const { return data_; }

private:
    QString name_;
    QtNodes::NodeDataType type_;
    std::shared_ptr<QtNodes::NodeData> data_;

    friend class NodePorts;
};

} // namespace nitro
