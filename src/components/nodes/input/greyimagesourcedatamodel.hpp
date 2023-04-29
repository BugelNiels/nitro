#pragma once

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QtNodes/NodeDelegateModel>

#include "cbdimage.hpp"
#include "src/components/nodes/imagedata.hpp"

namespace nitro {
    class GreyImageSourceDataModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

    public:
        GreyImageSourceDataModel();

        virtual ~GreyImageSourceDataModel() {}

    public:
        static QString nodeCaption() { return QStringLiteral("Greyscale Image Source"); }

        static QString nodeName() { return QStringLiteral("GreyImageSource"); }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }

    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public:
        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override {}

        QWidget *embeddedWidget() override;

    public:

    private Q_SLOTS:

        void onLoadButtonPressed();

    private:
        // TODO: create a super node class that has this param
        const int _embedImgSize = 128;
        std::shared_ptr<ImageData> _image;

        QString* _path;
        QWidget *_displayWrapper;
        QPushButton *_loadButton;
        QLabel *_imgLabel;

        void loadImage(const QString &filePath);
    };
}