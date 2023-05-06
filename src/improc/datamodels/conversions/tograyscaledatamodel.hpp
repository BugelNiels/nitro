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

        static QString nodeCaption() { return QStringLiteral("To Grayscale"); }
        static QString nodeName() { return QStringLiteral("ToGrayscale"); }
        static QString nodeIcon() { return QStringLiteral(":/icons/nodes/greyscale.png"); }
        static QColor nodeColor() { return {59, 90, 115}; }


        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }

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