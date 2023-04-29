#pragma once

#include "3rdparty/nodeeditor/include/QtNodes/NodeDelegateModel"

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>

#include "cbdimage.hpp"
#include "src/components/nodes/colimagedata.hpp"
#include "src/components/nodes/imagedata.hpp"

class QLineEdit;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.

namespace nitro {
    class ToGrayScaleDataModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

    public:
        ToGrayScaleDataModel();

        virtual ~ToGrayScaleDataModel() {}

    public:

        static QString nodeCaption() { return QStringLiteral("To Grayscale"); }
        static QString nodeName() { return QStringLiteral("ToGrayscale"); }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }

    public:
        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

        QWidget *embeddedWidget() override;

    private:

        const int _embedImgSize = 128;
        std::shared_ptr<ColImageData> _in;
        std::shared_ptr<ImageData> _result;

        QWidget *_displayWrapper;
        QLabel *_imgLabel;
    };
}