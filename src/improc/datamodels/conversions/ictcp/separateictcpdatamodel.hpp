#pragma once


#include "datamodels/conversions/colseparatedatamodel.hpp"
#include "src/core/nodes/nodeinfo.hpp"

namespace nitro {

    class SeparateICtCpDataModel : public ColorSeparateDataModel {
    public:
        explicit SeparateICtCpDataModel();

        ~SeparateICtCpDataModel() override = default;

        static NodeInfo nodeInfo() {
            return {"Separate ICtCp",
                    "SeparateICtCp",
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

