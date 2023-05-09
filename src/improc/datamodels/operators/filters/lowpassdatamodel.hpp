#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>

#include "src/improc/core/cbdimage.hpp"
#include "datamodels/imagedatamodel.hpp"

class QLineEdit;

namespace nitro {
    class LowPassFilterDataModel : public ImageDataModel {
    Q_OBJECT

    public:
        LowPassFilterDataModel();

        virtual ~LowPassFilterDataModel() {}

    public:
        static QString nodeCaption() { return QStringLiteral("Low Pass Filter"); }

        static QString nodeName() { return QStringLiteral("LowPassFilter"); }
        static QString nodeIcon() { return QStringLiteral(":/icons/nodes/threshold.png"); }
        static QColor nodeColor() { return {59, 90, 115}; }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }


        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public Q_SLOTS:


    protected:
        QWidget *initBeforeWidget() override;
        float cutoff = 20;
        int order = 5;

    private:

        std::shared_ptr<ImageData> _result;
        std::shared_ptr<ImageData> _input;

        void recompute();

        std::shared_ptr<ImageData> filter(CbdImage &image);

        QDoubleSpinBox *cutoffSpinBox;
        QSpinBox *orderSpinBox;
    };
}