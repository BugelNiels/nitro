#pragma once

#include "3rdparty/nodeeditor/include/QtNodes/NodeDelegateModel"

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>

#include "cbdimage.hpp"
#include "src/nodes/operators/imopdatamodel.hpp"

class QLineEdit;

namespace nitro {
    class KMeansDataModel : public ImOpDataModel {
    Q_OBJECT

    public:
        KMeansDataModel();

        ~KMeansDataModel() override = default;

    public:
        static QString nodeCaption() { return QStringLiteral("K-Means"); }

        static QString nodeName() { return QStringLiteral("KMeans"); }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }


    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public Q_SLOTS:

        void kValChanged(int val);

        void changeDither(bool toggled);
        void iterValChanged(int val);

    protected:

        std::shared_ptr<ImageData> compute(const QImage &inputImg) override;

        std::shared_ptr<ImageData> compute(const nitro::CbdImage &inputImg) override;

        QWidget *initBeforeWidget() override;

        QWidget *initAfterWidget() override;

    private:
        int numIterations = 4;
        bool dither = false;
        int k = 8;

        QSpinBox *kSpinBox;
        QSpinBox *iterSpinBox;
        QCheckBox *ditherCheckBox;
    };
}