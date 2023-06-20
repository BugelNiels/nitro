#include "nodes/nodeports.hpp"
#include "QtNodes/InvalidData.hpp"
#include "QtNodes/NodeData"
#include "nodes/datatypes/integerdata.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include "QtNodes/Definitions"

#include <utility>

namespace nitro {
    NodePorts::NodePorts() = default;

    NodePorts::NodePorts(std::vector<PortData> inputList,
                         std::vector<PortData> outputList) {
        for (auto &portData: inputList) {
            inputList_.push_back(portData.name_);
            inputMap_[portData.name_] = std::move(portData);
        }
        for (auto &portData: outputList) {
            outputList_.push_back(portData.name_);
            outputMap_[portData.name_] = std::move(portData);
        }
    }

    NodePorts::~NodePorts() = default;


    void NodePorts::setOutputData(const QString &name, std::shared_ptr<QtNodes::NodeData> data) {
        if (outputMap_.count(name) == 0) {
            std::cerr << "setOutputData: Node output does not contain port with name: " << name.toStdString()
                      << std::endl;
            return;
        }
        outputMap_[name].type_ = data->type();
        outputMap_[name].data_ = std::move(data);
    }

    QtNodes::NodeDataType NodePorts::inDataType(QtNodes::PortIndex port) const {
        if (port >= numInPorts()) {
            return QtNodes::InvalidData().type();
        }
        return inputMap_.at(inputList_[port]).type_;
    }

    QtNodes::NodeDataType NodePorts::outDataType(QtNodes::PortIndex port) const {
        if (port >= numOutPorts()) {
            return QtNodes::InvalidData().type();
        }
        return outputMap_.at(outputList_[port]).type_;
    }

    int NodePorts::numInPorts() const {
        return inputList_.size();
    }

    int NodePorts::numOutPorts() const {
        return outputList_.size();
    }

    const QString &NodePorts::inPortName(QtNodes::PortIndex port) const {
        return inputMap_.at(inputList_[port]).name_;
    }

    const QString &NodePorts::outPortName(QtNodes::PortIndex port) const {
        return outputMap_.at(outputList_[port]).name_;
    }

    std::shared_ptr<QtNodes::NodeData> NodePorts::getOutData(const QString &name) {
        return outputMap_[name].data_;
    }

    std::shared_ptr<QtNodes::NodeData> NodePorts::getInData(const QString &name) const {
        return inputMap_.at(name).data_;
    }

    std::shared_ptr<QtNodes::NodeData> NodePorts::getOutData(QtNodes::PortIndex portIndex) {
        return getOutData(outPortName(portIndex));
    }

    void NodePorts::setOutputType(QtNodes::PortIndex port, QtNodes::NodeDataType type) {
        if (port == QtNodes::InvalidPortIndex || port >= numOutPorts()) {
            return;
        }
        outputMap_[outputList_[port]].type_ = std::move(type);
    }

    void NodePorts::setInData(QtNodes::PortIndex port, std::shared_ptr<QtNodes::NodeData> data) {
        if (port == QtNodes::InvalidPortIndex || port >= numInPorts()) {
            return;
        }
        inputMap_[inPortName(port)].data_ = std::move(data);
    }

    int NodePorts::inputInteger(const QString &name) const {
        if (inputMap_.count(name) == 0) {
            return 0;
        }
        auto inputValDat = std::dynamic_pointer_cast<nitro::IntegerData>(inputMap_.at(name).data_);
        if (inputValDat == nullptr) {
            return 0;
        }
        return inputValDat->value();
    }

    double NodePorts::inputValue(const QString &name) const {
        if (inputMap_.count(name) == 0) {
            return 0;
        }
        auto inputValDat = std::dynamic_pointer_cast<nitro::DecimalData>(inputMap_.at(name).data_);
        if (inputValDat == nullptr) {
            return 0;
        }
        return inputValDat->value();
    }

    bool NodePorts::allInputsPresent() {
        bool present = true;
        for (auto &[key, value]: inputMap_) {
            auto ptr = getInData(value.name_);
            if (ptr == nullptr || ptr->empty()) {
                present = false;
                break;
            }
        }
        // Reset all output
        if (!present) {
            for (auto &[key, value]: outputMap_) {
                value.data_ = nullptr;
            }
        }
        return present;
    }

    void NodePorts::setGlobalProperty(const QString &key, QString value) {
        properties_[key] = std::move(value);
    }

    QString NodePorts::getGlobalProperty(const QString &key) {
        if (properties_.count(key) > 0) {
            return properties_[key];
        }
        return "";
    }
} // nitro
