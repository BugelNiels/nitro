#pragma once


#include "datamodels/conversions/colcombinedatamodel.hpp"

namespace nitro {

    class CombineRgbDataModel : public ColorCombineDataModel {
    public:
        explicit CombineRgbDataModel();

        ~CombineRgbDataModel() override = default;


        static QString nodeCaption() { return QStringLiteral("Combine RGB"); }

        static QString nodeName() { return QStringLiteral("CombineRgb"); }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }


    protected:

        QWidget *embeddedWidget() override { return nullptr; }

        void combine(const QVector<std::shared_ptr<ImageData>> &input) override;

    };

} // nitro
