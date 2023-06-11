#pragma once

#include <vector>
#include <QString>
#include <iostream>
#include <map>
#include <opencv2/core/mat.hpp>

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

        [[nodiscard]] const QString& inPortName(int port) const;
        [[nodiscard]] const QString& outPortName(int port) const;

        [[nodiscard]] QtNodes::NodeDataType inDataType(int port) const;
        [[nodiscard]] QtNodes::NodeDataType outDataType(int port) const;

        [[nodiscard]] int numInPorts() const;
        [[nodiscard]] int numOutPorts() const;

        std::shared_ptr<QtNodes::NodeData> getInData(int portIndex);
        std::shared_ptr<QtNodes::NodeData> getOutData(int portIndex);

        std::shared_ptr<QtNodes::NodeData> getInData(const QString& name);
        std::shared_ptr<QtNodes::NodeData> getOutData(const QString& name);

        int getInputInteger(const QString& name, bool& present);
        double getInputValue(const QString& name, bool& present);
        std::shared_ptr<cv::Mat> getInputImage(const QString& name, bool& present);

        void setOutputInteger(const QString& name, int val);
        void setOutputValue(const QString& name, double val);
        void setOutputImage(const QString& name, const std::shared_ptr<cv::Mat>& im);


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
