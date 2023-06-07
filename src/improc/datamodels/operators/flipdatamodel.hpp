#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>

#include "src/core/cbdimage.hpp"
#include "src/improc/datamodels/imagedatamodel.hpp"
#include "nodes/datatypes/imagedata.hpp"

class QLineEdit;

namespace nitro {
    class FlipDataModel : public ImageDataModel {
    Q_OBJECT

    public:
        FlipDataModel();

        ~FlipDataModel() override = default;

    public:
        static QtNodes::NodeInfo nodeInfo() {
            return {"FLIP",
                    "Flip",
                    {118, 185, 0},
                    ":/icons/nodes/compare.png"};
        }

        QString caption() const override { return nodeInfo().getNodeName(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeInfo().getNodeId(); }

    protected:

        QWidget *initBeforeWidget() override;

        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

    private:

        std::shared_ptr<ImageData> _inputImgA;
        std::shared_ptr<ImageData> _inputImgB;

        std::shared_ptr<ImageData> _result;


        void setFlipResult();

        QLabel* mseLabel;
    };
}