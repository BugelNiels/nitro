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

    class NodePorts {
    public:
        NodePorts();

        NodePorts(std::vector<PortData> inputList_,
                  std::vector<PortData> outputList_);

        ~NodePorts();

        [[nodiscard]] const QString &inPortName(QtNodes::PortIndex port) const;

        [[nodiscard]] const QString &outPortName(QtNodes::PortIndex port) const;

        [[nodiscard]] QtNodes::NodeDataType inDataType(QtNodes::PortIndex port) const;

        [[nodiscard]] QtNodes::NodeDataType outDataType(QtNodes::PortIndex port) const;

        [[nodiscard]] int numInPorts() const;

        [[nodiscard]] int numOutPorts() const;

        [[nodiscard]] std::shared_ptr<QtNodes::NodeData> getOutData(QtNodes::PortIndex portIndex);

        [[nodiscard]] std::shared_ptr<QtNodes::NodeData> getInData(const QString &name) const;

        [[nodiscard]] std::shared_ptr<QtNodes::NodeData> getOutData(const QString &name);

        [[nodiscard]] int inputInteger(const QString &name) const;

        [[nodiscard]] double inputValue(const QString &name) const;

        [[nodiscard]] const std::shared_ptr<QtNodes::NodeData> &inGet(const QString &name) const {
            if (inputMap_.count(name) == 0) {
                throw std::invalid_argument(
                        QString("Input port with name: %1 does not exist.").arg(name).toStdString());
            }
            return inputMap_.at(name).getData();
        }

        [[nodiscard]] const std::shared_ptr<QtNodes::NodeData> &outGet(const QString &name) const {
            if (outputMap_.count(name) == 0) {
                throw std::invalid_argument(
                        QString("Output port with name: %1 does not exist.").arg(name).toStdString());
            }
            return outputMap_.at(name).getData();
        }

        template<class T>
        [[nodiscard]] const T &inGet(const QString &name) const {
            if (inputMap_.count(name) == 0) {
                throw std::invalid_argument(
                        QString("Input port with name: %1 does not exist.").arg(name).toStdString());
            }
            auto inDat = inputMap_.at(name).data_;
            if (inDat == nullptr) {
                throw std::invalid_argument(QString("Attempting to retrieve nullptr: %1.").arg(name).toStdString());
            }
            auto dat = std::dynamic_pointer_cast<T>(inDat);
            if (dat == nullptr) {
                throw std::invalid_argument(
                        QString("Failed to retrieve input port of type %1 for input port  \"%2\".\n"
                                "\tItem is of type: %3").arg(T::dataInfo().getDataName(),
                                                             name, inDat->type().name).toStdString());
            }
            return *dat;
        }

        template<class T>
        [[nodiscard]] const T &outGet(const QString &name) {
            if (outputMap_.count(name) == 0) {
                throw std::invalid_argument(
                        QString("Output port with name: %1 does not exist.").arg(name).toStdString());
            }
            auto inDat = outputMap_.at(name).data_;
            if (inDat == nullptr) {
                // TODO: this probs don't work
                outputMap_[name] = std::make_shared<T>();
                inDat =  outputMap_.at(name).data_;
            }
            auto dat = std::dynamic_pointer_cast<T>(inDat);
            if (dat == nullptr) {
                throw std::invalid_argument(
                        QString("Failed to retrieve input port of type %1 for input port  \"%2\".\n"
                                "\tItem is of type: %3").arg(T::dataInfo().getDataName(),
                                                             name, inDat->type().name).toStdString());
            }
            return *dat;
        }

        template<class T>
        [[nodiscard]] bool inputOfType(const std::shared_ptr<QtNodes::NodeData> &t) const {
            return dynamic_cast<std::shared_ptr<T>>(t) != nullptr;
        }

        template<class T>
        [[nodiscard]] bool inputOfType(const QString& name) const {
            return std::dynamic_pointer_cast<T>(inputMap_.at(name).data_) != nullptr;
        }

        template<class T>
        [[nodiscard]] bool allInputOfType() const {
            for(const auto& [key, value] : inputMap_) {
                if(std::dynamic_pointer_cast<T>(value.data_) == nullptr) {
                    return false;
                }
            }
            return true;
        }

        template<class T, class U>
        void output(const QString &name, const U &data) {
            setOutputData(name, std::make_shared<T>(T(data)));
        }

        void setOutputData(const QString &name, std::shared_ptr<QtNodes::NodeData> data);

        void setOutputType(QtNodes::PortIndex port, QtNodes::NodeDataType type);

        void setInData(QtNodes::PortIndex port, std::shared_ptr<QtNodes::NodeData> data);

        bool allInputsPresent();

        void setGlobalProperty(const QString& key, QString value);

        QString getGlobalProperty(const QString& key);

    private:
        std::vector<QString> inputList_;
        std::vector<QString> outputList_;
        std::map<QString, PortData> inputMap_;
        std::map<QString, PortData> outputMap_;

        std::map<QString, QString> properties_;

    };


} // nitro
