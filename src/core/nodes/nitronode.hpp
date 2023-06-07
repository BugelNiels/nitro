#pragma once

#include <QString>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include "QtNodes/NodeDelegateModel"
#include "src/core/nodes/operators/nodeoperator.hpp"
#include "3rdparty/nodeeditor/include/QtNodes/NodeInfo.hpp"

namespace nitro {

    class NitroNode : public QtNodes::NodeDelegateModel {

    public:
        // TODO: check lifetime of these references

        NitroNode() = default;

        ~NitroNode() override;

        void init(QtNodes::NodeInfo info,
                  const NodePorts& nodePorts,
                  std::shared_ptr<NodeOperator> algo,
                  QWidget *widget);

        [[nodiscard]] QString caption() const override;

        [[nodiscard]] bool captionVisible() const override;

        [[nodiscard]] QString name() const override;

        QWidget *embeddedWidget() override;

        [[nodiscard]] const QtNodes::NodeInfo &getInfo() const;

    protected:

        [[nodiscard]] unsigned int nPorts(QtNodes::PortType portType) const override;

        [[nodiscard]] QtNodes::NodeDataType
        dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

    private:

        void connectSpinBox(QSpinBox * spinBox, int port);

        QtNodes::NodeInfo info_;
        std::shared_ptr<NodeOperator> algo_;
        NodePorts nodePorts_;

        // TODO: extract the map part into separate class for safe retrieval of elements

        QWidget *widget_;

        friend class NitroNodeBuilder;

        void connectInputWidget(QSpinBox *spinBox, int port);

        void connectInputWidget(QDoubleSpinBox *spinBox, int port);

        void connectLoadButton(QPushButton *button, int port);
    };

} // nitro
