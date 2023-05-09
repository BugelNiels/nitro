#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>

#include "src/improc/core/cbdimage.hpp"
#include "datamodels/imagedatamodel.hpp"

class QLineEdit;

namespace nitro {
    class ToGrayScaleDataModel : public ImageDataModel {
    Q_OBJECT

    public:
        ToGrayScaleDataModel();

        ~ToGrayScaleDataModel() override = default;

    public:

        static NodeInfo nodeInfo() {
            return {"To Grayscale",
                    "ToGrayscale",
                    {131, 131, 131},
                    ":/icons/nodes/greyscale.png"};
        }

        QString caption() const override { return nodeInfo().getNodeName(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeInfo().getNodeId(); }

    public:
        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

    private:

        const int _embedImgSize = 128;
        std::shared_ptr<ImageData> _in;
        std::shared_ptr<ImageData> _result;

        QWidget *_displayWrapper;
        QLabel *_imgLabel;
    };
}