#pragma once

#include "3rdparty/nodeeditor/include/QtNodes/NodeDelegateModel"

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>

#include "src/improc/core/cbdimage.hpp"
#include "datamodels/operators/imopdatamodel.hpp"

class QLineEdit;

namespace nitro {
    class SlrDataModel : public ImOpDataModel {
    Q_OBJECT

    public:
        SlrDataModel();
        ~SlrDataModel() override = default;

    public:
        static QString nodeCaption() { return QStringLiteral("Smart Layer Removal"); }

        static QString nodeName() { return QStringLiteral("SLR"); }
        static QString nodeIcon() { return QStringLiteral(":/icons/nodes/quantisize.png"); }
        static QColor nodeColor() { return {95, 120, 83}; }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public Q_SLOTS:

        void kValChanged();

    protected:

        std::shared_ptr<ImageData> compute(const QImage &inputImg) override;

        std::shared_ptr<ImageData> compute(const nitro::CbdImage &inputImg) override;

        QWidget *initBeforeWidget() override;

    private:
        int k = 8;

        QSpinBox *kSpinBox;
    };
}