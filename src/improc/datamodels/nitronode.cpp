//
// Created by niels on 4-6-23.
//

#include "nitronode.hpp"

#include <utility>
#include "invaliddata.hpp"

namespace nitro {

    NitroNode::NitroNode(NodeInfo info,
                         const std::vector<std::pair<QString, QtNodes::NodeDataType>> &input,
                         const std::vector<std::pair<QString, QtNodes::NodeDataType>> &output,
                         std::map<QString, std::shared_ptr<QtNodes::NodeData>>& inputMap,
                         std::map<QString, std::shared_ptr<QtNodes::NodeData>>& outputMap,
                         std::shared_ptr<NodeAlgorithm> algo,
                         QWidget *widget)
            : info_(std::move(info)),
              algo_(std::move(algo)),
              inputList_(input),
              outputList_(output),
              inputMap_(inputMap),
              outputMap_(outputMap),
              widget_(widget) {
    }

    NitroNode::~NitroNode() {
    }

    QString NitroNode::caption() const {
        return info_.getNodeName();
    }

    bool NitroNode::captionVisible() const {
        return true;
    }

    QString NitroNode::name() const {
        return info_.getNodeId();
    }

    unsigned int NitroNode::nPorts(QtNodes::PortType portType) const {
        if (portType == QtNodes::PortType::In) {
            return inputList_.size();
        }
        if (portType == QtNodes::PortType::Out) {
            return outputList_.size();
        }
        return 0;
    }

    QtNodes::NodeDataType NitroNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
        if (portType == QtNodes::PortType::In) {
            if (portIndex >= inputList_.size()) {
                return InvalidData().type();
            }
            return inputList_[portIndex].second;
        }
        if (portType == QtNodes::PortType::Out) {
            if (portIndex >= outputList_.size()) {
                return InvalidData().type();
            }
            return outputList_[portIndex].second;
        }
        return InvalidData().type();
    }

    std::shared_ptr<QtNodes::NodeData> NitroNode::outData(QtNodes::PortIndex portIndex) {
        return outputMap_[outputList_[portIndex].first];
    }

    void NitroNode::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
        if (data == nullptr) {
            // Do something?
            // Set result to null?
        }
        inputMap_[inputList_[portIndex].first] = data;
        std::map<QString, QString> options;
        algo_->compute(inputMap_, outputMap_, options);

        for (int i = 0; i < outputList_.size(); i++) {
            // For now just emit that everything has been updated
            Q_EMIT dataUpdated(i);
        }


    }

    QWidget *NitroNode::embeddedWidget() {
        return widget_;
    }

    const NodeInfo &NitroNode::getInfo() const {
        return info_;
    }

} // nitro