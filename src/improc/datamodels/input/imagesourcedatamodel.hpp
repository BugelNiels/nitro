#pragma once

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QtNodes/NodeDelegateModel>

#include "src/improc/core/cbdimage.hpp"
#include "datamodels/imagedatamodel.hpp"

namespace nitro {
    class ImageSourceDataModel : public ImageDataModel {
        Q_OBJECT

    public:
        ImageSourceDataModel();

        ~ImageSourceDataModel() override = default;

        static NodeInfo nodeInfo() {
            return {"Image Source",
                    "ImageSource",
                    {121, 70, 29},
                    ":/icons/nodes/image_source.png"};
        }

        QString caption() const override { return nodeInfo().getNodeName(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeInfo().getNodeId(); }

        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override {}

    protected:
        QWidget *initBeforeWidget() override;

        QWidget *initAfterWidget() override;

    private
        Q_SLOTS:

                void onLoadButtonPressed();

    private:
        std::shared_ptr<ImageData> _image;

        QString *_path;
        QPushButton *_loadButton;

        void loadImage(const QString &filePath);

    };
}