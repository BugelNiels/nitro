#pragma once

#include <QString>
#include <QLabel>
#include "QtNodes/NodeDelegateModel"
#include "src/improc/core/algorithms/nodealgorithm.hpp"
#include "nodes/nodeinfo.hpp"

namespace nitro {

    class NitroNode : public QtNodes::NodeDelegateModel {

    public:
        // TODO: check lifetime of these references
        NitroNode(NodeInfo info,
                  const std::vector<std::pair<QString, QtNodes::NodeDataType>> &input,
                  const std::vector<std::pair<QString, QtNodes::NodeDataType>> &output,
                  std::map<QString, std::shared_ptr<QtNodes::NodeData>>& inputMap,
                  std::map<QString, std::shared_ptr<QtNodes::NodeData>>& outputMap,
                  std::shared_ptr<NodeAlgorithm> algo,
                  QWidget *widget);


        NitroNode() = default;
        ~NitroNode() override;

        [[nodiscard]] QString caption() const override;

        [[nodiscard]] bool captionVisible() const override;

        [[nodiscard]] QString name() const override;

        QWidget *embeddedWidget() override;

        [[nodiscard]] const NodeInfo &getInfo() const;

    protected:

        [[nodiscard]] unsigned int nPorts(QtNodes::PortType portType) const override;

        [[nodiscard]] QtNodes::NodeDataType
        dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

    private:
        NodeInfo info_;
        const std::shared_ptr<NodeAlgorithm> algo_;
        std::vector<std::pair<QString, QtNodes::NodeDataType>> inputList_;
        std::vector<std::pair<QString, QtNodes::NodeDataType>> outputList_;
        std::map<QString, std::shared_ptr<QtNodes::NodeData>> inputMap_;
        std::map<QString, std::shared_ptr<QtNodes::NodeData>> outputMap_;
        QWidget *widget_;

        friend class NitroNodeBuilder;
    };

} // nitro
