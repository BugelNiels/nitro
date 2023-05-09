#pragma once
#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>


#include "src/improc/core/cbdimage.hpp"
#include "src/improc/datamodels/datatypes/imagedata.hpp"

namespace nitro {
    class ColorCombineDataModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

    public:
        explicit ColorCombineDataModel(int numChannels);

        ~ColorCombineDataModel() override = default;

    public:
        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;


    protected:

        virtual void combine(const QVector<std::shared_ptr<ImageData>> &input) = 0;

        std::shared_ptr<ImageData> _result;

    private:
        QVector<std::shared_ptr<ImageData>> _input;


    };
}