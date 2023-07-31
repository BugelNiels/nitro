#pragma once

#include "nitronode.hpp"
#include "nodeoperator.hpp"
#include <QColor>
#include <QLabel>
#include <QString>
#include <set>
#include <valueslider.hpp>

class QVBoxLayout;

using BoundMode = ValueSliders::BoundMode;

namespace nitro {

/**
 * @brief Builder class that can be used to build new node types. All nodes should be constructed via this builder.
 * The builder allows for the chaining of different method to easily create unique node configurations.
 *
 */
class NitroNodeBuilder {

public:
    /**
     * @brief Creates a new builder.
     * @param name The name of the node (as displayed in the UI).
     * @param id The id of the node. This should be unique among all nodes.
     * @param category Category the node belongs to.
     */
    NitroNodeBuilder(QString name, QString id, QString category);

    /**
     * @brief Creates the node as configured.
     * @return A new node.
     */
    std::unique_ptr<NitroNode> build();

    /**
     * @brief Adds an operator to the node. The operator is what is executed when a node is being evaluated.
     * @param op The operator of the node.
     * @return The updated builder.
     */
    NitroNodeBuilder *withOperator(std::unique_ptr<NodeOperator> op);

    /**
     * @brief Gives the node a specific color. Affects the icon color and caption background of a node.
     * @param color The color the node should have.
     * @return The updated builder.
     */
    NitroNodeBuilder *withNodeColor(const QColor &color);

    /**
     * @brief Gives the node an icon. The icon path should be present in one of the resource files.
     * @param path The path to the icon.
     * @return The updated builder.
     */
    NitroNodeBuilder *withIcon(const QString &path);

    /**
     * @brief Gives the node an input port that takes inputs of type T.
     * @see nitro::NodePorts#inputInteger
     * @see nitro::NodePorts#inputValue
     * @see nitro::NodePorts#inGet
     * @see nitro::NodePorts#inGetAs
     * @see nitro::NodePorts#getInData
     * @tparam T The input type. Should be one of the data types (e.g. subtype of NodeDataType or FlexibleData). @see nitro::FlexibleData.
     * @param name The name of the port. Should be unique amongst the input ports, since this is used to retrieve the corresponding data.
     * @return The updated builder.
     */
    template<class T>
    NitroNodeBuilder *withInputPort(const QString &name) {
        QtNodes::NodeDataType imDataType = T().type();
        inputList_.emplace_back(name, imDataType);
        addInPortWidget(new QLabel(name));
        return this;
    }

    /**
     * @brief Gives the node an input port that takes inputs of type T. The port, however, also accepts any data type with the id present in the allowedConversions list.
     * @see nitro::NodePorts#inputInteger
     * @see nitro::NodePorts#inputValue
     * @see nitro::NodePorts#inGet
     * @see nitro::NodePorts#inGetAs
     * @see nitro::NodePorts#getInData
     * Note that this does not perform the actual conversions, it simply allows for the port to accept these types.
     * @tparam T The input type. Should be one of the data types (e.g. subtype of NodeDataType or FlexibleData). @see nitro::FlexibleData.
     * @param name The name of the port. Should be unique amongst the input ports, since this is used to retrieve the corresponding data.
     * @param allowedConversions The type IDs of the data types this port should also be able to accept.
     * @return The updated builder.
     */
    template<class T>
    NitroNodeBuilder *withInputPort(const QString &name,
                                    std::initializer_list<QString> allowedConversions) {
        QtNodes::NodeDataType imDataType = T().type();
        for (auto &type: allowedConversions) {
            imDataType.allowedFromConversions.insert(type);
        }
        inputList_.emplace_back(name, imDataType);
        addInPortWidget(new QLabel(name));
        return this;
    }

    /**
     * @brief Unique variation of withInputPort for integer types. Adds a slider widget to the node itself.
     * @see nitro::NodePorts#inputInteger
     * @see nitro::NodePorts#inGet
     * @see nitro::NodePorts#inGetAs
     * @see nitro::NodePorts#getInData
     * @param name The name of the port. Should be unique amongst the input ports, since this is used to retrieve the corresponding data.
     * @param defaultVal The default value of the input port/slider.
     * @param min The minimum value of the input port/slider.
     * @param max The maximum value of the input port/slider.
     * @param boundMode Whether or not the input value should be bounded or not.
     * @param allowedConversions The type IDs of the data types this port should also be able to accept.
     * @return The updated builder.
     */
    NitroNodeBuilder *withInputInteger(const QString &name,
                                       int defaultVal,
                                       int min,
                                       int max,
                                       BoundMode boundMode = BoundMode::UPPER_LOWER,
                                       std::initializer_list<QString> allowedConversions = {});

    /**
     * @brief Unique variation of withInputPort for double types. Adds a slider widget to the node itself.
     * @see nitro::NodePorts#inputValue
     * @see nitro::NodePorts#inGet
     * @see nitro::NodePorts#inGetAs
     * @see nitro::NodePorts#getInData
     * @param name The name of the port. Should be unique amongst the input ports, since this is used to retrieve the corresponding data.
     * @param defaultVal The default value of the input port/slider.
     * @param min The minimum value of the input port/slider.
     * @param max The maximum value of the input port/slider.
     * @param boundMode Whether or not the input value should be bounded or not.
     * @param allowedConversions The type IDs of the data types this port should also be able to accept.
     * @return The updated builder.
     */
    NitroNodeBuilder *withInputValue(const QString &name,
                                     double defaultVal,
                                     double min,
                                     double max,
                                     BoundMode boundMode = BoundMode::UPPER_LOWER,
                                     std::initializer_list<QString> allowedConversions = {});

    /**
     * @brief Gives the node an output port that produces outputs of type T. Note that this type could be changed by the node operator if necessary.
     * In other words, there are technically no restrictions on what the operator can output.
     * @see nitro::NodePorts#output
     * @see nitro::NodePorts#setOutputData
     * @tparam T The output type. Should be one of the data types (e.g. subtype of NodeDataType or FlexibleData). @see nitro::FlexibleData.
     * @param name The name of the port. Should be unique amongst the output ports, since this is used to put the corresponding data.
     * @return The updated builder.
     */
    template<class T>
    NitroNodeBuilder *withOutputPort(const QString &name) {
        QtNodes::NodeDataType imDataType = T().type();
        outputList_.emplace_back(name, imDataType);
        addOutPortWidget(new QLabel(name));
        return this;
    }

    /**
     * @brief Gives the node a load button to load a particular data type.
     * @see nitro::NodePorts#setGlobalProperty
     * @see nitro::NodePorts#getGlobalProperty
     * @param name The name of the button. Should be unique amongst the options, since this is used to retrieve the corresponding data.
     * @param filters File filters associated with the type that should be loaded.
     * @return The updated builder.
     */
    NitroNodeBuilder *withLoadButton(const QString &name, const QString &filters);

    /**
     * @brief Adds an output port producing integers to the node.
     * Unique variation of withOutputPort for integer types. @see nitro::NitroNodeBuilder#withOutputPort.
     * @see nitro::NodePorts#output
     * @see nitro::NodePorts#setOutputData
     * @see nitro::NitroNodeBuilder#withOutputPort.
     * @param name he name of the port. Should be unique amongst the output ports, since this is used to put the corresponding data.
     * @return The updated builder.
     */
    NitroNodeBuilder *withOutputInteger(const QString &name);

    /**
     * @brief Adds an output port producing integers to the node.
     * Unique variation of withOutputPort for integer types. Produces the given default value if nothing is present.
     * @see nitro::NodePorts#output
     * @see nitro::NodePorts#setOutputData
     * @see nitro::NitroNodeBuilder#withOutputPort.
     * @param name The name of the port. Should be unique amongst the output ports, since this is used to put the corresponding data.
     * @param defaultVal The default value to produce if the output is empty.
     * @return The updated builder.
     */
    NitroNodeBuilder *withOutputInteger(const QString &name, int defaultVal);

    /**
     * @brief Adds an output port producing doubles to the node.
     * Unique variation of withOutputPort for double types. @see nitro::NitroNodeBuilder#withOutputPort.
     * * @see nitro::NodePorts#output
     * @see nitro::NodePorts#setOutputData
     * @see nitro::NitroNodeBuilder#withOutputPort.
     * @param name he name of the port. Should be unique amongst the output ports, since this is used to put the corresponding data.
     * @return The updated builder.
     */
    NitroNodeBuilder *withOutputValue(const QString &name);

    /**
     * @brief Adds an output port producing doubles to the node.
     * Unique variation of withOutputPort for double types. Produces the given default value if nothing is present.
     * @see nitro::NodePorts#output
     * @see nitro::NodePorts#setOutputData
     * @see nitro::NitroNodeBuilder#withOutputPort.
     * @param name The name of the port. Should be unique amongst the output ports, since this is used to put the corresponding data.
     * @param defaultVal The default value to produce if the output is empty.
     * @return The updated builder.
     */
    NitroNodeBuilder *withOutputValue(const QString &name, double defaultVal);

    /**
     * @brief Special port that produces a value not via the node operator by via a slider widget. Adds a slider widget to the node that produces integer values.
     * @param name The name of the port. Should be unique amongst the output ports, since this is used to put the corresponding data.
     * @param defaultVal The default value of the input port/slider.
     * @param min The minimum value of the output port/slider.
     * @param max The maximum value of the output port/slider.
     * @param boundMode Whether or not the output value should be bounded or not.
     * @return The updated builder.
     */
    NitroNodeBuilder *withSourcedOutputInteger(const QString &name,
                                               int defaultVal,
                                               int min,
                                               int max,
                                               BoundMode boundMode = BoundMode::UPPER_LOWER);

    /**
     * @brief Special port that produces a value not via the node operator by via a slider widget. Adds a slider widget to the node that produces double values.
     * @param name The name of the port. Should be unique amongst the output ports, since this is used to put the corresponding data.
     * @param defaultVal The default value of the input port/slider.
     * @param min The minimum value of the output port/slider.
     * @param max The maximum value of the output port/slider.
     * @param boundMode Whether or not the output value should be bounded or not.
     * @return The updated builder.
     */
    NitroNodeBuilder *withSourcedOutputValue(const QString &name,
                                             double defaultVal,
                                             double min,
                                             double max,
                                             BoundMode boundMode = BoundMode::UPPER_LOWER);

    /**
     * @brief Adds a dropdown widget to the node.
     * @see nitro::NodePorts#getOption
     * @param name The name of the dropdown. Should be unique amongst the options, since this is used to retrieve the corresponding data.
     * @param options The string options present in the dropdown.
     * @return The updated builder.
     */
    NitroNodeBuilder *withDropDown(const QString &name, const QStringList &options);

    /**
     * @brief Adds a checkbox widget to the node.
     * @see nitro::NodePorts#optionEnabled
     * @param name The name of the checkbox. Should be unique amongst the options, since this is used to retrieve the corresponding data.
     * @param checked Whether the checkbox should be initially checked or not.
     * @return The updated builder.
     */
    NitroNodeBuilder *withCheckBox(const QString &name, bool checked = false);

    /**
     * @brief Adds a simple label to the node. Can be used to display images by adding a PixMap to the label.
     * @param name The name of the label. Should be unique amongst the options, since this is used to put the corresponding data.
     * @param label The label to add to the node.
     * @return The updated builder.
     */
    NitroNodeBuilder *withDisplayWidget(const QString &name, QLabel *label);

    /**
     * @brief Adds a simple label with the provided text to the node.
     * @param name The name of the label. Should be unique amongst the options, since this is used to put the corresponding data.
     * @param text The text to add to the node.
     * @return The updated builder.
     */
    NitroNodeBuilder *withDisplayWidget(const QString &name, const QString &text);

private:
    const int portWidgetWidth_ = 150;

    const QString name_;
    const QString id_;
    const QString category_;
    std::unique_ptr<NodeOperator> algo_;
    std::unique_ptr<NitroNode> node_;
    int portWidgetHeight_;

    QColor nodeColor_ = NITRO_CONVERTER_COLOR;
    QString iconPath_;

    std::vector<PortData> inputList_;
    std::vector<PortData> outputList_;
    std::unordered_map<QString, int> options_;
    QVBoxLayout *inLayout_;
    QVBoxLayout *outLayout_;
    QVBoxLayout *optionLayout_;

    void addInPortWidget(QWidget *widget);

    void addInPortWidget(QLabel *label);

    void addOutPortWidget(QWidget *widget);

    void addOutPortWidget(QLabel *label);

    void addOptionWidget(QWidget *widget);

    void initInputValue(const QString &name,
                        ValueSliders::IntSlider *slider,
                        std::initializer_list<QString> list);

    void initInputVal(const QString &name,
                      ValueSliders::DoubleSlider *slider,
                      std::initializer_list<QString> list);
};

} // namespace nitro
