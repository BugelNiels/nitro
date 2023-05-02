#pragma once

#include "3rdparty/nodeeditor/include/QtNodes/NodeDelegateModel"

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>

#include "cbdimage.hpp"
#include "src/nodes/operators/imopdatamodel.hpp"

class QLineEdit;

namespace nitro {
    class QuantisizeDataModel : public ImOpDataModel {
    Q_OBJECT

    public:
        QuantisizeDataModel();
        ~QuantisizeDataModel() override = default;

    public:
        static QString nodeCaption() { return QStringLiteral("Quantisize"); }

        static QString nodeName() { return QStringLiteral("Quantisize"); }
        static QString nodeIcon() { return QStringLiteral(":/icons/nodes/quantisize.png"); }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public Q_SLOTS:

        void kValChanged();

        void changeDither(bool toggled);

    protected:

        std::shared_ptr<ImageData> compute(const QImage &inputImg) override;

        std::shared_ptr<ImageData> compute(const nitro::CbdImage &inputImg) override;

        QWidget *initBeforeWidget() override;

        QWidget *initAfterWidget() override;

    private:
        bool dither = false;
        int k = 8;

        QSpinBox *kSpinBox;
        QCheckBox *ditherCheckBox;
    };
}