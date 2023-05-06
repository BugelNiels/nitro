#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>

#include "src/improc/core/cbdimage.hpp"
#include "datamodels/operators/imopdatamodel.hpp"

#include "core/algorithms/distancefield.hpp"

class QLineEdit;

namespace nitro {

    enum SampleMethod {
        LINEAR, CUBIC_HERMITE, CUBIC_INTERPOLATORY
    };

    class ResampleDataModel : public ImOpDataModel {
    Q_OBJECT

    public:
        ResampleDataModel();

        ~ResampleDataModel() override {}

    public:
        static QString nodeCaption() { return QStringLiteral("Resample"); }

        static QString nodeName() { return QStringLiteral("Resample"); }

        static QString nodeIcon() { return QStringLiteral(":/icons/nodes/resample.png"); }

        static QColor nodeColor() { return {201, 94, 6}; }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }


    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public Q_SLOTS:

        void modeChanged(int mode);

        void targetValChanged();

    protected:

        std::shared_ptr<ImageData> compute(const QImage &inputImg) override;

        std::shared_ptr<ImageData> compute(const nitro::CbdImage &inputImg) override;

        QWidget *initBeforeWidget() override;

        QWidget *initAfterWidget() override;

        void clearData() override;

    private:
        DistanceField *field;
        SampleMethod _mode = SampleMethod::LINEAR;
        int targetK = 255;

        QComboBox *modeCombobox;
        QSpinBox *targetSpinBox;

    };
}