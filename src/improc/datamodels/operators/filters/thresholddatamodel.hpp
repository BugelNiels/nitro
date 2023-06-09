#pragma once

#include "external/nodeeditor/include/QtNodes/NodeDelegateModel"

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>

#include "src/core/cbdimage.hpp"
#include "src/improc/datamodels/operators/imopdatamodel.hpp"

class QLineEdit;

namespace nitro {
    class ThresholdDataModel : public ImOpDataModel {
        Q_OBJECT

    public:
        ThresholdDataModel();

        virtual ~ThresholdDataModel() {}


        static QtNodes::NodeInfo nodeInfo() {
            return {"Threshold",
                    "Threshold",
                    {43, 101, 43},
                    ":/icons/nodes/threshold.png"};
        }

        QString caption() const override { return nodeInfo().getNodeName(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeInfo().getNodeId(); }

        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public Q_SLOTS:

        void modeChanged(int mode);

        void thresholdValChanged();

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