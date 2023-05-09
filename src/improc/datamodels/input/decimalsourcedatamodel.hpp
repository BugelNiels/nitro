#pragma once

#include <QtCore/QObject>

#include <QLabel>
#include <QSpinBox>
#include "3rdparty/nodeeditor/include/QtNodes/NodeDelegateModel"

#include "datamodels/integerdata.hpp"
#include "nodeeditor/nodeinfo.hpp"

namespace nitro {
    class DecimalSourceDataModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

    public:
        DecimalSourceDataModel();

        ~DecimalSourceDataModel() override = default;

    public:
        static NodeInfo nodeInfo() {
            return {"Value",
                    "Value",
                    {131, 49, 74},
                    ":/icons/nodes/number.png"};
        }

        QString caption() const override { return nodeInfo().getNodeName(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeInfo().getNodeId(); }

    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public:
        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override {}

    protected:
        QWidget *embeddedWidget() override;


    private:
        QDoubleSpinBox* spinBox;
        QWidget *_displayWrapper = nullptr;
    };
}