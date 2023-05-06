#pragma once


#include "datamodels/conversions/colseparatedatamodel.hpp"

namespace nitro {

    class SeparateYCbCrDataModel : public ColorSeparateDataModel {
    public:
        explicit SeparateYCbCrDataModel();

        ~SeparateYCbCrDataModel() override = default;


        static QString nodeCaption() { return QStringLiteral("Separate YCbCr"); }

        static QString nodeName() { return QStringLiteral("SeparateYCbCr"); }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }


    protected:

        QWidget *embeddedWidget() override { return nullptr; }

        void separate(const QImage &inputImg) override;

    };

} // nitro

