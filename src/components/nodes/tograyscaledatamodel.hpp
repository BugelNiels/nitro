#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>

#include "cbdimage.hpp"
#include "colimagedata.hpp"
#include "imagedata.hpp"

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
        QString caption() const override { return QStringLiteral("To Grayscale"); }

        bool captionVisible() const override { return true; }

        QString name() const override { return QStringLiteral("ToGrayscale"); }

    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public:
        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

        QWidget *embeddedWidget() override;

    private:

        const int _embedImgSize = 256;
        std::shared_ptr<ColImageData> _in;
        std::shared_ptr<ImageData> _result;

        QWidget *_displayWrapper;
        QLabel *_imgLabel;
    };
}