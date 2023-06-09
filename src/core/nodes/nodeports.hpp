#pragma once

#include <vector>
#include <QString>
#include <iostream>
#include "QtNodes/NodeData"

namespace nitro {

    class NodePorts {
    public:
        NodePorts();
        NodePorts(std::vector<std::pair<QString, QtNodes::NodeDataType>> inputList_,
        std::vector<std::pair<QString, QtNodes::NodeDataType>> outputList_,
        std::map<QString, std::shared_ptr<QtNodes::NodeData>> inputMap_,
        std::map<QString, std::shared_ptr<QtNodes::NodeData>> outputMap_);

        ~NodePorts();

        [[nodiscard]] const QString& inPortName(int port) const;
        [[nodiscard]] const QString& outPortName(int port) const;

        [[nodiscard]] QtNodes::NodeDataType inDataType(int port) const;
        [[nodiscard]] QtNodes::NodeDataType outDataType(int port) const;

        [[nodiscard]] int numInPorts() const;
        [[nodiscard]] int numOutPorts() const;

        template<class T>
        std::shared_ptr<T> getInDataAsType(const QString& name) {
            if (inputMap_.count(name) == 0) {
                return nullptr;
            }
            return std::dynamic_pointer_cast<T>(inputMap_.at(name));
        }

        std::shared_ptr<QtNodes::NodeData> getInData(int portIndex);
        std::shared_ptr<QtNodes::NodeData> getOutData(int portIndex);

        std::shared_ptr<QtNodes::NodeData> getInData(const QString& name);
        std::shared_ptr<QtNodes::NodeData> getOutData(const QString& name);


        void setOutputData(const QString& name, std::shared_ptr<QtNodes::NodeData> data);
        void setOutputType(int port, QtNodes::NodeDataType type);

        void setInData(int port, std::shared_ptr<QtNodes::NodeData> data);

    private:
        std::vector<std::pair<QString, QtNodes::NodeDataType>> inputList_;
        std::vector<std::pair<QString, QtNodes::NodeDataType>> outputList_;
        std::map<QString, std::shared_ptr<QtNodes::NodeData>> inputMap_;
        std::map<QString, std::shared_ptr<QtNodes::NodeData>> outputMap_;

    };


} // nitro
