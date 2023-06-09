#pragma once

#include <QtCore/QObject>

#include <QLabel>
#include <QSpinBox>
#include "external/nodeeditor/include/QtNodes/NodeDelegateModel"

#include "external/nodeeditor/include/QtNodes/NodeInfo.hpp"

namespace nitro {
    class DecimalSourceDataModel : public QtNodes::NodeDelegateModel {
        Q_OBJECT

    public:
        DecimalSourceDataModel();

        ~DecimalSourceDataModel() override = default;

        static QtNodes::NodeInfo nodeInfo() {
            return {"Value",
                    "Value",
                    {131, 49, 74},
                    ":/icons/nodes/number.png"};
        }

        QString caption() const override { return nodeInfo().getNodeName(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeInfo().getNodeId(); }

        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override {}

    protected:
        QWidget *embeddedWidget() override;


    private:
        QDoubleSpinBox *spinBox;
        QWidget *_displayWrapper = nullptr;
    };
}