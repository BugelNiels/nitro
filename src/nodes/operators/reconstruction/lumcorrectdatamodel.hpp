#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>

#include "cbdimage.hpp"
#include "src/nodes/imagedatamodel.hpp"

class QLineEdit;

namespace nitro {
    class LuminanceCorrectionDataModel : public ImageDataModel {
    Q_OBJECT

    public:
        LuminanceCorrectionDataModel();

        virtual ~LuminanceCorrectionDataModel() {}

    public:
        static QString nodeCaption() { return QStringLiteral("Luminance Correction"); }

        static QString nodeName() { return QStringLiteral("LuminanceCorrection"); }
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

        void modeChanged(int mode);

    protected:
        QWidget *initBeforeWidget() override;
        int _mode = 0;

    private:
        QComboBox *modeCombobox;


        std::shared_ptr<ImageData> _result;
        std::shared_ptr<ImageData> _input;
        std::shared_ptr<ImageData> _inputRef;

        void recompute();

        std::shared_ptr<ImageData> colorBrightnessCorrect(QImage &image, CbdImage &image1);

        std::shared_ptr<ImageData> brightnessCorrect(CbdImage &image, CbdImage &image1);
    };
}