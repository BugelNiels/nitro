#pragma once


#include "datamodels/conversions/colseparatedatamodel.hpp"

namespace nitro {

    class SeparateRgbDataModel : public ColorSeparateDataModel {
    public:
        explicit SeparateRgbDataModel();

        ~SeparateRgbDataModel() override = default;


        static QString nodeCaption() { return QStringLiteral("Separate RGB"); }

        static QString nodeName() { return QStringLiteral("SeperateRgb"); }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }


    protected:

        QWidget *embeddedWidget() override { return nullptr; }

        void separate(const QImage &inputImg) override;

    };

} // nitro

