#include "nodes/nodeports.hpp"
#include "QtNodes/InvalidData.hpp"
#include "QtNodes/NodeData"
#include "nodes/datatypes/imagedata.hpp"
#include "nodes/datatypes/integerdata.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include "QtNodes/Definitions"

#include <utility>

#include <QDebug>

namespace nitro {
    NodePorts::NodePorts() = default;

    NodePorts::NodePorts(std::vector<std::pair<QString, QtNodes::NodeDataType>> inputList,
                         std::vector<std::pair<QString, QtNodes::NodeDataType>> outputList,
                         std::map<QString, std::shared_ptr<QtNodes::NodeData>> inputMap,
                         std::map<QString, std::shared_ptr<QtNodes::NodeData>> outputMap)
            : inputList_(std::move(inputList)),
              outputList_(std::move(outputList)),
              inputMap_(std::move(inputMap)),
              outputMap_(std::move(outputMap)) {

    }

    NodePorts::~NodePorts() = default;


    void NodePorts::setOutputData(const QString &name, std::shared_ptr<QtNodes::NodeData> data) {
        if (outputMap_.count(name) == 0) {
            std::cerr << "setOutputData: Node output does not contain port with name: " << name.toStdString()
                      << std::endl;
            return;
        }
        outputMap_[name] = std::move(data);
    }

    QtNodes::NodeDataType NodePorts::inDataType(QtNodes::PortIndex port) const {
        if (port >= numInPorts()) {
            return QtNodes::InvalidData().type();
        }
        return inputList_[port].second;
    }

    QtNodes::NodeDataType NodePorts::outDataType(QtNodes::PortIndex port) const {
        if (port >= numOutPorts()) {
            return QtNodes::InvalidData().type();
        }
        return outputList_[port].second;
    }

    int NodePorts::numInPorts() const {
        return inputList_.size();
    }

    int NodePorts::numOutPorts() const {
        return outputList_.size();
    }

    const QString &NodePorts::inPortName(QtNodes::PortIndex port) const {
        return inputList_[port].first;
    }

    const QString &NodePorts::outPortName(QtNodes::PortIndex port) const {
        return outputList_[port].first;
    }

    std::shared_ptr<QtNodes::NodeData> NodePorts::getOutData(const QString &name) {
        return outputMap_[name];
    }

    std::shared_ptr<QtNodes::NodeData> NodePorts::getInData(const QString &name) const {
        return inputMap_.at(name);
    }

    std::shared_ptr<QtNodes::NodeData> NodePorts::getInData(QtNodes::PortIndex portIndex) const {
        return getInData(inPortName(portIndex));
    }

    std::shared_ptr<QtNodes::NodeData> NodePorts::getOutData(QtNodes::PortIndex portIndex) {
        return getOutData(outPortName(portIndex));
    }

    void NodePorts::setOutputType(QtNodes::PortIndex port, QtNodes::NodeDataType type) {
        if (port == QtNodes::InvalidPortIndex || port >= numOutPorts()) {
            return;
        }
        outputList_[port].second = std::move(type);
    }

    void NodePorts::setInData(QtNodes::PortIndex port, std::shared_ptr<QtNodes::NodeData> data) {
        if (port == QtNodes::InvalidPortIndex || port >= numInPorts()) {
            return;
        }
        inputMap_[inPortName(port)] = std::move(data);
    }

    int NodePorts::getInputInteger(const QString &name) const {
        if (inputMap_.count(name) == 0) {
            return 0;
        }
        auto inputValDat = std::dynamic_pointer_cast<nitro::IntegerData>(inputMap_.at(name));
        if (inputValDat == nullptr) {
            return 0;
        }
        return inputValDat->value();
    }

    double NodePorts::getInputValue(const QString &name) const {
        if (inputMap_.count(name) == 0) {
            return 0;
        }
        auto inputValDat = std::dynamic_pointer_cast<nitro::DecimalData>(inputMap_.at(name));
        if (inputValDat == nullptr) {
            return 0;
        }
        return inputValDat->value();
    }

    std::shared_ptr<cv::Mat> NodePorts::getInputImage(const QString &name) const {
        if (inputMap_.count(name) == 0) {
            return nullptr;
        }
        auto inputImgDat = std::dynamic_pointer_cast<nitro::ImageData>(inputMap_.at(name));
        if (inputImgDat == nullptr) {
            return nullptr;
        }
        auto img = inputImgDat->image();
        if (img == nullptr) {
            return nullptr;
        }
        return img;
    }

    void NodePorts::setOutputInteger(const QString &name, int val) {
        setOutputData(name, std::make_shared<nitro::IntegerData>(val));
    }

    void NodePorts::setOutputValue(const QString &name, double val) {
        setOutputData(name, std::make_shared<nitro::DecimalData>(val));
    }

    void NodePorts::setOutputImage(const QString &name, const std::shared_ptr<cv::Mat> &im) {
        setOutputData(name, std::make_shared<nitro::ImageData>(im));
    }

    bool NodePorts::inputsPresent(std::initializer_list<QString> list) {
        bool present = true;
        for (auto &item: list) {
            auto ptr = getInData(item);
            if (ptr == nullptr) {
                present = false;
                break;
            }
        }
        // Reset all output
        if (!present) {
            for (auto &entry: outputMap_) {
                entry.second = nullptr;
            }
        }
        return present;
    }


} // nitro
