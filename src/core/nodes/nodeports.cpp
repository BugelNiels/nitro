#include "nodes/nodeports.hpp"
#include "QtNodes/InvalidData.hpp"
#include "QtNodes/NodeData"
#include "nodes/datatypes/imagedata.hpp"
#include "nodes/datatypes/integerdata.hpp"
#include "nodes/datatypes/decimaldata.hpp"

#include <utility>

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

    QtNodes::NodeDataType NodePorts::inDataType(int port) const {
        if (port >= numInPorts()) {
            return QtNodes::InvalidData().type();
        }
        return inputList_[port].second;
    }

    QtNodes::NodeDataType NodePorts::outDataType(int port) const {
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

    const QString &NodePorts::inPortName(int port) const {
        return inputList_[port].first;
    }

    const QString &NodePorts::outPortName(int port) const {
        return outputList_[port].first;
    }

    std::shared_ptr<QtNodes::NodeData> NodePorts::getOutData(const QString &name) {
        return outputMap_[name];
    }

    std::shared_ptr<QtNodes::NodeData> NodePorts::getInData(const QString &name) {
        return inputMap_[name];
    }

    std::shared_ptr<QtNodes::NodeData> NodePorts::getInData(int portIndex) {
        return getInData(inPortName(portIndex));
    }

    std::shared_ptr<QtNodes::NodeData> NodePorts::getOutData(int portIndex) {
        return getOutData(outPortName(portIndex));
    }

    void NodePorts::setOutputType(int port, QtNodes::NodeDataType type) {
        if (port >= numOutPorts()) {
            std::cerr << "setOutputType: Invalid port index for output data setting. Port: " << port << "Output size: "
                      << outputList_.size() << std::endl;
            return;
        }
        outputList_[port].second = std::move(type);
    }

    void NodePorts::setInData(int port, std::shared_ptr<QtNodes::NodeData> data) {
        if (port >= numInPorts()) {
            std::cerr << "setInData: Invalid port index for input data setting. Port: " << port << "Input size: "
                      << inputList_.size() << std::endl;
            return;
        }
        inputMap_[inPortName(port)] = std::move(data);
    }

    int NodePorts::getInputInteger(const QString &name, bool &present) {
        present = false;
        if (inputMap_.count(name) == 0) {
            return 0;
        }
        auto inputValDat = std::dynamic_pointer_cast<nitro::IntegerData>(inputMap_.at(name));
        if (inputValDat == nullptr) {
            return 0;
        }
        present = true;
        return inputValDat->value();
    }

    double NodePorts::getInputValue(const QString &name, bool &present) {
        present = false;
        if (inputMap_.count(name) == 0) {
            return 0;
        }
        auto inputValDat = std::dynamic_pointer_cast<nitro::DecimalData>(inputMap_.at(name));
        if (inputValDat == nullptr) {
            return 0;
        }
        present = true;
        return inputValDat->value();
    }

    std::shared_ptr<cv::Mat> NodePorts::getInputImage(const QString &name, bool &present) {
        present = false;
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
        present = true;
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


} // nitro
