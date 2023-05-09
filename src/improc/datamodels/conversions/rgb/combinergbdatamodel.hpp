#pragma once


#include "datamodels/conversions/colcombinedatamodel.hpp"
#include "nodeeditor/nodeinfo.hpp"

namespace nitro {

    class CombineRgbDataModel : public ColorCombineDataModel {
    public:
        explicit CombineRgbDataModel();

        ~CombineRgbDataModel() override = default;

        static NodeInfo nodeInfo() {
            return {"Combine RGB",
                    "CombineRGB",
                    {36, 98, 131},
                    ":/icons/nodes/combine.png"};
        }

        QString caption() const override { return nodeInfo().getNodeName(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeInfo().getNodeId(); }

    protected:

        QWidget *embeddedWidget() override { return nullptr; }

        void combine(const QVector<std::shared_ptr<ImageData>> &input) override;

    };

} // nitro
