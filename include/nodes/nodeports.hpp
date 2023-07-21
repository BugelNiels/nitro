#pragma once

#include <utility>
#include <vector>
#include <QString>
#include <iostream>
#include <map>
#include <opencv2/core/mat.hpp>
#include "QtNodes/Definitions"
#include "nodes/datatypes/flexibledata.hpp"
#include "portdata.hpp"

namespace QtNodes {
class NodeData;

class NodeDataType;
}

namespace nitro {

/**
 * Class that encapsulates the following information of a particular node:
 * - Input data/ports
 * - Output data/ports
 * - Any additional node options, as defined by e.g. dropdowns in the view.
 */
class NodePorts {
public:
    /**
     * Creates an empty collection of ports.
     */
    NodePorts();

    /**
     * Creates a new collection of ports with the desired input data, output data and options.
     * @param inputList Ordered list of ports that should function as the input ports of the node in question.
     * @param outputList Ordered list of ports that should function as the output ports of the node in question.
     * @param options Options that describe the options used when evaluating the node operation.
     */
    NodePorts(std::vector<PortData> inputList,
              std::vector<PortData> outputList,
              std::unordered_map<QString, int> options);

    /**
     * Destructor.
     */
    ~NodePorts();

    /**
     * Retrieves the name of an input port at the provided index.
     * @param port The index of the port to retrieve the name of.
     * @return Name of the input port.
     */
    [[nodiscard]] const QString &inPortName(QtNodes::PortIndex port) const;

    /**
     * Retrieves the name of an output port at the provided index.
     * @param port The index of the port to retrieve the name of.
     * @return Name of the output port.
     */
    [[nodiscard]] const QString &outPortName(QtNodes::PortIndex port) const;

    /**
     * Retrieves the data type of an input port at the provided index.
     * @param port The index of the port to retrieve the data type of.
     * @return Data type of the input port.
     */
    [[nodiscard]] QtNodes::NodeDataType inDataType(QtNodes::PortIndex port) const;

    /**
     * Retrieves the data type of an output port at the provided index.
     * @param port The index of the port to retrieve the data type of.
     * @return Data type of the output port.
     */
    [[nodiscard]] QtNodes::NodeDataType outDataType(QtNodes::PortIndex port) const;

    /**
     * Retrieves the number of input ports.
     * @return The number of input ports.
     */
    [[nodiscard]] int numInPorts() const;

    /**
     * Retrieves the number of output ports.
     * @return The number of output ports.
     */
    [[nodiscard]] int numOutPorts() const;

    /**
     * Retrieves a pointer to the data of the output port at the provided index.
     * This data can be changed to change the output of the node.
     * @param portIndex The index of the output port at which to retrieve the data.
     * @return A pointer to the data associated with the output port at the provided index.
     */
    [[nodiscard]] std::shared_ptr<QtNodes::NodeData> getOutData(QtNodes::PortIndex portIndex);

    /**
     * Retrieves a pointer to the data of the input port with the provided name.
     * This data cannot be changed here. Use setInData for this instead. @see setInData.
     * @param name The name of the input port at which to retrieve the data.
     * @return A pointer to the data associated with the input port with the provided name.
     */
    [[nodiscard]] std::shared_ptr<QtNodes::NodeData> getInData(const QString &name) const;

    /**
     * Retrieves a pointer to the data of the output port with the provided name.
     * This data can be changed to change the output of the node.
     * @param name The name of the output port at which to retrieve the data.
     * @return A pointer to the data associated with the output port with the provided name.
     */
    [[nodiscard]] std::shared_ptr<QtNodes::NodeData> getOutData(const QString &name);

    /**
     * Retrieves an integer value from the input port with the provided name.
     * Equivalent to calling: *nodePorts.inGetAs<IntegerData>(name);
     * @see NodePorts::inGetAs.
     * @param name Name of the input port.
     * @return The integer value located at the input port with name "name".
     */
    [[nodiscard]] int inputInteger(const QString &name) const;

    /**
     * Retrieves a double value from the input port with the provided name.
     * Equivalent to calling: *nodePorts.inGetAs<DecimalData>(name);
     * @see NodePorts::inGetAs.
     * @param name Name of the input port.
     * @return The double value located at the input port with name "name".
     */
    [[nodiscard]] double inputValue(const QString &name) const;

    /**
     * Retrieves the generic NodeData from the input port with the provided name.
     * Ideally, use nodePorts.inGetAs instead to prevent manual and error-prone casting.
     * @see NodePorts::inGetAs.
     * @param name Name of the input port.
     * @return The generic node data located at the input port with name "name".
     */
    [[nodiscard]] const std::shared_ptr<QtNodes::NodeData> &inGet(const QString &name) const {
        if (inputMap_.count(name) == 0) {
            throw std::invalid_argument(
                    QString("Input port with name: %1 does not exist.").arg(name).toStdString());
        }
        return inputMap_.at(name).getData();
    }

    /**
     * Retrieves the node data of type T from the input port with the provided name.
     * Note that this data should be convertable to T, otherwise this will throw an error.
     * @see FlexibleData::from.
     * @tparam T Type of the data that should be retrieved.
     * Does not necessarily need to be equal to the data type that is stored in the node, but it should be possible to convert between T and the data stored in the node.
     * @param name Name of the input port.
     * @return The node data located at the input port with name "name".
     */
    template<typename T>
    [[nodiscard]] typename T::DataType inGetAs(const QString &name) const {
        if (inputMap_.count(name) == 0) {
            throw std::invalid_argument(
                    QString("Input port with name: %1 does not exist.").arg(name).toStdString());
        }
        return T::from(inputMap_.at(name).getData());
    }

    /**
     * Checks whether the data at the input port with the name "name" is of type T.
     * @tparam T The type to check the data at the input port against.
     * @param name The name of the input port.
     * @return True if the data at input port "name" is of type T. False otherwise.
     */
    template<class T>
    [[nodiscard]] bool inputOfType(const QString &name) const {
        return std::dynamic_pointer_cast<T>(inputMap_.at(name).data_) != nullptr;
    }

    /**
     * Checks whether the data at all input ports are of type T.
     * @tparam T The type to check the data at the input ports against.
     * @return True if the data at all input ports is of type T. False otherwise.
     */
    template<class T>
    [[nodiscard]] bool allInputOfType() const {
        for (const auto &[key, value]: inputMap_) {
            if (std::dynamic_pointer_cast<T>(value.data_) == nullptr) {
                return false;
            }
        }
        return true;
    }

    /**
     * Sets the data for an output port. All data is encapsulated by a custom type. For example, IntegerDataType encapsulated integers.
     * This function allows you to directly output an integer as an IntegerData without doing all the intermediate steps.
     * @tparam T The type of the data that the output should be of. In the example, this would be IntegerData.
     * @tparam U The type of the data that is encapsulated by the output port. This does not need to be set explicitly and can be inferred by the compiler. In the example, this would be int.
     * @param name The name of the output port.
     * @param data The data to set.
     */
    template<class T, class U>
    void output(const QString &name, const U &data) {
        setOutputData(name, std::make_shared<T>(T(data)));
    }

    /**
     * Get the option associated with the provided name.
     * @param optionName Name of the option.
     * @return The integer option.
     */
    int getOption(const QString &optionName);

    /**
     * Checks whether the option with the provided name is enabled.
     * @param optionName Name of the option.
     * @return True if the option is enabled. False otherwise.
     */
    bool optionEnabled(const QString &optionName);

    /**
     * Sets the value of an option.
     * @param optionName Name of the option.
     * @param val Value to set the option to.
     */
    void setOption(const QString &optionName, int val);

    /**
     * Sets generic input data.
     * @param port The index of the input port to set the data at.
     * @param data The data to set at the input port with the provided index.
     */
    void setInData(QtNodes::PortIndex port, std::shared_ptr<QtNodes::NodeData> data);

    /**
     * Checks if all the input ports contain valid data.
     * @return True if all the input ports contain valid data (e.g. no null pointers). False otherwise.
     */
    bool allInputsPresent();

    /**
     * Sets a global property of the node. This differs from the options in that these global properties are not associated with a visual component.
     * @param key The key describing this property. Should be unique.
     * @param value The corresponding value.
     */
    void setGlobalProperty(const QString &key, QString value);

    /**
     * Retrieves a global property.
     * @param key The key of the property to retrieve.
     * @return The property associated with the key. Returns "" if the key does not exist.
     */
    QString getGlobalProperty(const QString &key);

private:
    std::vector<QString> inputList_;
    std::vector<QString> outputList_;
    std::unordered_map<QString, PortData> inputMap_;
    std::unordered_map<QString, PortData> outputMap_;
    std::unordered_map<QString, QtNodes::NodeDataType> origInTypes_;

    std::unordered_map<QString, QString> properties_;
    std::unordered_map<QString, int> options_; // used for dropdowns and other options

    void setOutputData(const QString &name, std::shared_ptr<QtNodes::NodeData> data);
};

} // namespace nitro
