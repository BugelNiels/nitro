#pragma once


#include "datamodels/conversions/colcombinedatamodel.hpp"
#include "src/core/nodes/nodeinfo.hpp"

namespace nitro {

    class CombineYCbrCrDataModel : public ColorCombineDataModel {
    public:
        explicit CombineYCbrCrDataModel();

        ~CombineYCbrCrDataModel() override = default;

        static NodeInfo nodeInfo() {
            return {"Combine YCbCr",
                    "CombineYCbCr",
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
