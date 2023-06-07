#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>

#include "src/core/cbdimage.hpp"
#include "datamodels/imagedatamodel.hpp"
#include "nodes/datatypes/imagedata.hpp"

class QLineEdit;

namespace nitro {
    class LowPassFilterDataModel : public ImageDataModel {
        Q_OBJECT

    public:
        LowPassFilterDataModel();

        virtual ~LowPassFilterDataModel() {}


        static QtNodes::NodeInfo nodeInfo() {
            return {"Low-Pass Filter",
                    "LowPassfilter",
                    {43, 101, 43},
                    ":/icons/nodes/blur.png"};
        }

        QString caption() const override { return nodeInfo().getNodeName(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeInfo().getNodeId(); }

        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

        QJsonObject save() const override;

        void load(QJsonObject const &p) override;


    protected:
        QWidget *initBeforeWidget() override;

        float cutoff = 20;
        int order = 5;

    private:

        std::shared_ptr<ImageData> _result;
        std::shared_ptr<ImageData> _input;

        void recompute();

        std::shared_ptr<ImageData> filter(CbdImage &image);

        QDoubleSpinBox *cutoffSpinBox;
        QSpinBox *orderSpinBox;
        QLabel *cutOffLabel;
        QLabel *orderLabel;
    };
}