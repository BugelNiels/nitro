#pragma once


#include "datamodels/conversions/colcombinedatamodel.hpp"

namespace nitro {

    class CombineYCbrCrDataModel : public ColorCombineDataModel {
    public:
        explicit CombineYCbrCrDataModel();

        ~CombineYCbrCrDataModel() override = default;


        static QString nodeCaption() { return QStringLiteral("Combine YCbCr"); }

        static QString nodeName() { return QStringLiteral("CombineYCbCr"); }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }


    protected:

        QWidget *embeddedWidget() override { return nullptr; }

        void combine(const QVector<std::shared_ptr<ImageData>> &input) override;

    };

} // nitro
