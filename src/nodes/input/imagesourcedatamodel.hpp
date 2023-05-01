#pragma once

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QtNodes/NodeDelegateModel>

#include "cbdimage.hpp"
#include "src/nodes/imagedata.hpp"
#include "src/nodes/imagedatamodel.hpp"

namespace nitro {
    class ImageSourceDataModel : public ImageDataModel {
    Q_OBJECT

    public:
        ImageSourceDataModel();

        ~ImageSourceDataModel() override = default;

    public:
        // TODO: change caption based on filename;
        static QString nodeCaption() { return QStringLiteral("Image Source"); }

        static QString nodeName() { return QStringLiteral("ImageSource"); }

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

    protected:
        QWidget *initBeforeWidget() override;
        QWidget *initAfterWidget() override;

    private Q_SLOTS:

        void onLoadButtonPressed();

    private:
        std::shared_ptr<ImageData> _image;

        QString *_path;
        QPushButton *_loadButton;

        void loadImage(const QString &filePath);

    };
}