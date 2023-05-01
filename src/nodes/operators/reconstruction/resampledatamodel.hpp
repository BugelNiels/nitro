#pragma once

#include "3rdparty/nodeeditor/include/QtNodes/NodeDelegateModel"

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>

#include "cbdimage.hpp"
#include "src/nodes/operators/imopdatamodel.hpp"

#include "src/common/util/distancefield.hpp"

class QLineEdit;

namespace nitro {

    enum SampleMethod {LINEAR, CUBIC_HERMITE, CUBIC_INTERPOLATORY};

    class ResampleDataModel : public ImOpDataModel {
    Q_OBJECT

    public:
        ResampleDataModel();

        virtual ~ResampleDataModel() {}

    public:
        static QString nodeCaption() { return QStringLiteral("Resample"); }

        static QString nodeName() { return QStringLiteral("Resample"); }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }


    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public Q_SLOTS:

        void modeChanged(int mode);

        void targetValChanged(int val);

    protected:

        std::shared_ptr<ImageData> compute(const QImage &inputImg) override;

        std::shared_ptr<ImageData> compute(const nitro::CbdImage &inputImg) override;

        QWidget *initBeforeWidget() override;

        QWidget *initAfterWidget() override;

        void clearData() override;

    private:
        DistanceField field;
        SampleMethod _mode = SampleMethod::LINEAR;
        int targetK = 255;

        QComboBox *modeCombobox;
        QSpinBox *targetSpinBox;

    };
}