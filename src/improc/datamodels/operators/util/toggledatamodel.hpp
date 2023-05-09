#pragma once

#include <QCheckBox>
#include "datamodels/imagedatamodel.hpp"

namespace nitro {

    class ToggleDataModel : public ImageDataModel {
    public:
        ToggleDataModel();

        ~ToggleDataModel() override = default;

    public:
        static NodeInfo nodeInfo() {
            return {"Toggle",
                    "Toggle",
                    {36, 98, 131},
                    ":/icons/nodes/toggle.png"};
        }

        QString caption() const override { return nodeInfo().getNodeName(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeInfo().getNodeId(); }
        
    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;


    protected:
        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

    protected:

        QWidget *initBeforeWidget() override;

    private:

        std::shared_ptr<ImageData> _inputImgA;
        std::shared_ptr<ImageData> _inputImgB;

        std::shared_ptr<ImageData> _result;

        QCheckBox *toggleCheckBox;

        void compute();
    };

} // nitro
