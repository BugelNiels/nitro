#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>

class DecimalData;

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

#include "cbdimage.hpp"
#include "colimagedata.hpp"

class QLineEdit;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.

namespace nitro {
    class ImageSourceDataModel : public NodeDelegateModel {
    Q_OBJECT

    public:
        ImageSourceDataModel();

        virtual ~ImageSourceDataModel() {}

    public:
        QString caption() const override { return QStringLiteral("Image Source"); }

        bool captionVisible() const override { return true; }

        QString name() const override { return QStringLiteral("ImageSource"); }

    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public:
        unsigned int nPorts(PortType portType) const override;

        NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

        std::shared_ptr<NodeData> outData(PortIndex port) override;

        void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

        QWidget *embeddedWidget() override;

    public:

    private Q_SLOTS:
        void onLoadButtonPressed();

    private:

        const int _embedImgSize = 256;
        QWidget* _displayWrapper;
        std::shared_ptr<ColImageData> _image;

        QPushButton *_loadButton;
        QLabel *_pathLabel;
    };
}