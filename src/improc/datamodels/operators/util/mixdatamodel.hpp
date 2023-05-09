#pragma once

#include <QComboBox>
#include "datamodels/imagedatamodel.hpp"
#include <QPainter>

namespace nitro {

    class MixDataModel : public ImageDataModel {
    public:
        MixDataModel();

        ~MixDataModel() override = default;

    public:
        static QString nodeCaption() { return QStringLiteral("Color Mix"); }

        static QString nodeName() { return QStringLiteral("ColMix"); }

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

        std::shared_ptr<QImage> _inputImgA;
        std::shared_ptr<QImage> _inputImgB;

        std::shared_ptr<ImageData> _result;

        QComboBox *modeBox;

        QMap<QString, QPainter::CompositionMode> supportedModes;

        void compute();
    };

} // nitro
