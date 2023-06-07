#pragma once


#include "datamodels/conversions/colseparatedatamodel.hpp"
#include "3rdparty/nodeeditor/include/QtNodes/NodeInfo.hpp"

namespace nitro {

    class SeparateYCbCrDataModel : public ColorSeparateDataModel {
    public:
        explicit SeparateYCbCrDataModel();

        ~SeparateYCbCrDataModel() override = default;

        static QtNodes::NodeInfo nodeInfo() {
            return {"Separate YCbCr",
                    "SeparateYCbCr",
                    {36, 98, 131},
                    ":/icons/nodes/separate.png"};
        }

        QString caption() const override { return nodeInfo().getNodeName(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeInfo().getNodeId(); }

    protected:

        QWidget *embeddedWidget() override { return nullptr; }

        void separate(const QImage &inputImg) override;

    };

} // nitro

