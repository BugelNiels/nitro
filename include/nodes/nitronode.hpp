#pragma once

#include "nodeoperator.hpp"
#include "nodeports.hpp"
#include <QString>
#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeInfo.hpp>

class QLabel;

class QPushButton;

class QComboBox;

class QCheckBox;

namespace ValueSliders {
class IntSlider;

class DoubleSlider;
} // namespace ValueSliders

namespace nitro {

/**
 * @brief The nodes used by NITRO. These should only be constructed using the NitroNodeBuilder.
 * @see nitro::NitroNodeBuilder
 */
class NitroNode : public QtNodes::NodeDelegateModel {

public:
    /**
     * @brief Creates a new empty node.
     */
    NitroNode() = default;

    /**
     * @brief Destructor.
     */
    ~NitroNode() override;

    /**
     * @brief Initialises this node with info.
     * @param info The node information.
     * @param nodePorts The node ports containing the information about the input/output ports and options.
     * @param op The node operator.
     * @param widget The display widget for this node as constructed by the builder.
     */
    void init(QtNodes::NodeInfo info,
              const NodePorts &nodePorts,
              std::unique_ptr<NodeOperator> op,
              QWidget *widget);

    /**
     * @brief Retrieves the caption of this node.
     * @return The caption of this node.
     */
    [[nodiscard]] QString caption() const override;

    /**
     * @brief Retrieves whether or not the caption should be visible or not.
     * @return Whether the caption of the node should be visible.
     */
    [[nodiscard]] bool captionVisible() const override;

    /**
     * @brief Retrieves the unique id of the node.
     * @return The id of this node.
     */
    [[nodiscard]] QString name() const override;

    /**
     * @brief Retrieves the widget displayed in this node.
     * @return The widget displayed in this node.
     */
    QWidget *embeddedWidget() override;

    /**
     * @brief Retrieves the node info.
     * @return The node info.
     */
    [[nodiscard]] const QtNodes::NodeInfo &getInfo() const;

    /**
     * @brief Saves this node information to JSON.
     * @return A JSON string containing the node data.
     */
    [[nodiscard]] QJsonObject save() const override;

    /**
     * @brief Updated the node with the information from the JSON string.
     * @see nitro::NitroNode#save
     * @param json A JSON string containing the node data.
     */
    void load(const QJsonObject &json) override;

public Q_SLOTS:

    void inputConnectionCreated(const QtNodes::ConnectionId &) override;

    void inputConnectionDeleted(const QtNodes::ConnectionId &) override;

protected:
    [[nodiscard]] unsigned int nPorts(QtNodes::PortType portType) const override;

    [[nodiscard]] QtNodes::NodeDataType dataType(QtNodes::PortType portType,
                                                 QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

    void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

private:
    QtNodes::NodeInfo info_;
    std::unique_ptr<NodeOperator> algo_;
    NodePorts nodePorts_;
    QJsonObject propJson_;
    // when new widgets_ are added, they register a function that allows it to grab a value from the json list
    std::unordered_map<QString, std::function<void(QJsonValue)>> widgetsJson_;
    std::unordered_map<QString, QWidget *> widgets_;

    QWidget *widget_ = nullptr;
    bool sliderInputDeleted_ = false;

    void connectInputWidget(ValueSliders::IntSlider *slider, QLabel *valLabel, int port);

    void connectInputWidget(ValueSliders::DoubleSlider *slider, QLabel *valLabel, int port);

    void connectLoadButton(const QString &name,
                           QPushButton *button,
                           int port,
                           const QString &filter);

    void connectComboBox(const QString &name, QComboBox *comboBox);

    void connectCheckBox(const QString &name, QCheckBox *checkBox);

    void connectSourceInteger(ValueSliders::IntSlider *slider, int port);

    void connectSourceValue(ValueSliders::DoubleSlider *slider, int port);

    [[nodiscard]] QString getInPortKey(unsigned int portIndex) const;

    [[nodiscard]] QString getOutPortKey(unsigned int portIndex) const;

    void connectLabel(const QString &name, QLabel *label);

    friend class NitroNodeBuilder;
};

} // namespace nitro
