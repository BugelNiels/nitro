#pragma once


#include "datamodels/conversions/colseparatedatamodel.hpp"
#include "nodeeditor/nodeinfo.hpp"

namespace nitro {

    class SeparateRgbDataModel : public ColorSeparateDataModel {
    public:
        explicit SeparateRgbDataModel();

        ~SeparateRgbDataModel() override = default;

        static NodeInfo nodeInfo() {
            return {"Separate RGB",
                    "SeparateRGB",
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

