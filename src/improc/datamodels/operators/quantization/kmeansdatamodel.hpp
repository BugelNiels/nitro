#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>

#include "src/improc/core/cbdimage.hpp"
#include "datamodels/operators/imopdatamodel.hpp"

class QLineEdit;

namespace nitro {
    class KMeansDataModel : public ImOpDataModel {
    Q_OBJECT

    public:
        KMeansDataModel();

        ~KMeansDataModel() override = default;

    public:
        static NodeInfo nodeInfo() {
            return {"K-Means",
                    "KMeans",
                    {43, 101, 43},
                    ":/icons/nodes/quantisize.png"};
        }

        QString caption() const override { return nodeInfo().getNodeName(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeInfo().getNodeId(); }

    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public Q_SLOTS:

        void kValChanged();

        void changeDither(bool toggled);

        void iterValChanged();

    protected:

        std::shared_ptr<ImageData> compute(const QImage &inputImg) override;

        std::shared_ptr<ImageData> compute(const nitro::CbdImage &inputImg) override;

        QWidget *initBeforeWidget() override;

    private:
        int numIterations = 4;
        bool dither = false;
        int k = 8;

        QSpinBox *kSpinBox;
        QSpinBox *iterSpinBox;
        QCheckBox *ditherCheckBox;
    };
}