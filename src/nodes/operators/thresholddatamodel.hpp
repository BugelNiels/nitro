#pragma once

#include "3rdparty/nodeeditor/include/QtNodes/NodeDelegateModel"

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>

#include "cbdimage.hpp"
#include "imopdatamodel.hpp"

class QLineEdit;

namespace nitro {
    class ThresholdDataModel : public ImOpDataModel {
    Q_OBJECT

    public:
        ThresholdDataModel();

        virtual ~ThresholdDataModel() {}

    public:
        static QString nodeCaption() { return QStringLiteral("Threshold"); }

        static QString nodeName() { return QStringLiteral("Threshold"); }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }


    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public Q_SLOTS:

        void modeChanged(int mode);

        void thresholdValChanged(int val);

    protected:
        int _mode = 0;

        std::shared_ptr<ImageData> compute(const QImage &inputImg) override;

        std::shared_ptr<ImageData> compute(const nitro::CbdImage &inputImg) override;

        QWidget *initBeforeWidget() override;

        QWidget *initAfterWidget() override;

    private:
        int threshold = 128;

        QComboBox *modeCombobox;
        QSpinBox *thresholdSpinBox;
    };
}