#pragma once

#include "external/nodeeditor/include/QtNodes/NodeDelegateModel"

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>

#include "src/core/cbdimage.hpp"
#include "datamodels/operators/imopdatamodel.hpp"

class QLineEdit;

namespace nitro {
    class SlrDataModel : public ImOpDataModel {
    Q_OBJECT

    public:
        SlrDataModel();

        ~SlrDataModel() override = default;

    public:
        static QtNodes::NodeInfo nodeInfo() {
            return {"Smart Layer Removal",
                    "SLR",
                    {43, 101, 43},
                    ":/icons/nodes/quantisize.png"};
        }

        QString caption() const override { return nodeInfo().getNodeName(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeInfo().getNodeId(); }

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