#pragma once

#include <QCheckBox>
#include <QSlider>
#include "datamodels/imagedatamodel.hpp"

namespace nitro {

    class BlendDataModel : public ImageDataModel {
    public:
        BlendDataModel();

        ~BlendDataModel() override = default;

    public:
        static QString nodeCaption() { return QStringLiteral("Blend"); }

        static QString nodeName() { return QStringLiteral("Blend"); }

        static QString nodeIcon() { return QStringLiteral(":/icons/nodes/blend.png"); }

        static QColor nodeColor() { return {59, 90, 115}; }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }


    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;


    protected:
        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

    protected:

        QWidget *initBeforeWidget() override;

    private:

        std::shared_ptr<ImageData> _inputImgA;
        std::shared_ptr<ImageData> _inputImgB;

        std::shared_ptr<ImageData> _result;

        QSlider *blendSlider;

        void compute();
    };

} // nitro
