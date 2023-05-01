#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>

#include "cbdimage.hpp"
#include "src/nodes/imagedatamodel.hpp"

class QLineEdit;

namespace nitro {
    class FlipDataModel : public ImageDataModel {
    Q_OBJECT

    public:
        FlipDataModel();

        ~FlipDataModel() override = default;

    public:
        static QString nodeCaption() { return QStringLiteral("FLIP"); }

        static QString nodeName() { return QStringLiteral("Flip"); }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }


    protected:
        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

    private:

        std::shared_ptr<ImageData> _inputImgA;
        std::shared_ptr<ImageData> _inputImgB;


        std::shared_ptr<ImageData> _result;


        void setFlipResult();
    };
}