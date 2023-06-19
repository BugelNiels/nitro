#pragma once

#include <vector>
#include <QString>
#include <iostream>
#include <map>
#include <opencv2/core/mat.hpp>
#include "QtNodes/Definitions"

namespace QtNodes {
    class NodeData;

    class NodeDataType;
}

namespace nitro {

    class NodePorts {
    public:
        NodePorts();

        NodePorts(std::vector<std::pair<QString, QtNodes::NodeDataType>> inputList_,
                  std::vector<std::pair<QString, QtNodes::NodeDataType>> outputList_,
                  std::map<QString, std::shared_ptr<QtNodes::NodeData>> inputMap_,
                  std::map<QString, std::shared_ptr<QtNodes::NodeData>> outputMap_);

        ~NodePorts();

        [[nodiscard]] const QString &inPortName(QtNodes::PortIndex port) const;

        [[nodiscard]] const QString &outPortName(QtNodes::PortIndex port) const;

        [[nodiscard]] QtNodes::NodeDataType inDataType(QtNodes::PortIndex port) const;

        [[nodiscard]] QtNodes::NodeDataType outDataType(QtNodes::PortIndex port) const;

        [[nodiscard]] int numInPorts() const;

        [[nodiscard]] int numOutPorts() const;

        [[nodiscard]] std::shared_ptr<QtNodes::NodeData> getInData(QtNodes::PortIndex portIndex) const;

        [[nodiscard]] std::shared_ptr<QtNodes::NodeData> getOutData(QtNodes::PortIndex portIndex);

        [[nodiscard]] std::shared_ptr<QtNodes::NodeData> getInData(const QString &name) const;

        [[nodiscard]] std::shared_ptr<QtNodes::NodeData> getOutData(const QString &name);

        [[nodiscard]] bool inputsPresent(std::initializer_list<QString> list);

        [[nodiscard]] int getInputInteger(const QString &name) const;

        [[nodiscard]] double getInputValue(const QString &name) const;

        [[nodiscard]] std::shared_ptr<cv::Mat> getInputImage(const QString &name) const;

        template<class T>
        std::shared_ptr<T> getInputData(const QString &name) const {
            if (inputMap_.count(name) == 0) {
                return nullptr;
            }
            auto inDat = std::dynamic_pointer_cast<T>(inputMap_.at(name));
            if (inDat == nullptr) {
                return nullptr;
            }
            return inDat;
        }

        [[nodiscard]] std::shared_ptr<QtNodes::NodeData> get(const QString &name) const {
            if (inputMap_.count(name) == 0) {
                return nullptr;
            }
            return inputMap_.at(name);
        }

        void setOutputInteger(const QString &name, int val);

        void setOutputValue(const QString &name, double val);

        void setOutputImage(const QString &name, const std::shared_ptr<cv::Mat> &im);


        void setOutputData(const QString &name, std::shared_ptr<QtNodes::NodeData> data);

        void setOutputType(QtNodes::PortIndex port, QtNodes::NodeDataType type);

        void setInData(QtNodes::PortIndex port, std::shared_ptr<QtNodes::NodeData> data);

    private:
        std::vector<std::pair<QString, QtNodes::NodeDataType>> inputList_;
        std::vector<std::pair<QString, QtNodes::NodeDataType>> outputList_;
        std::map<QString, std::shared_ptr<QtNodes::NodeData>> inputMap_;
        std::map<QString, std::shared_ptr<QtNodes::NodeData>> outputMap_;

    };


} // nitro
