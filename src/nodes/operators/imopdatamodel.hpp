#pragma once

#include "3rdparty/nodeeditor/include/QtNodes/NodeDelegateModel"

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>


#include "cbdimage.hpp"
#include "src/nodes/imagedata.hpp"
#include "src/nodes/imagedatamodel.hpp"

namespace nitro {
    class ImOpDataModel : public ImageDataModel {
    Q_OBJECT

    public:
        ImOpDataModel();

        ~ImOpDataModel() override = default;

    public:
        bool captionVisible() const override { return true; }

    public:
        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

    protected:
        virtual void recompute();

        virtual void clearData();

        virtual std::shared_ptr<ImageData> compute(const QImage &inputImg) = 0;

        virtual std::shared_ptr<ImageData> compute(const nitro::CbdImage &inputImg) = 0;

    private:
        std::shared_ptr<ImageData> _result;
        std::shared_ptr<ImageData> _input;


    };
}