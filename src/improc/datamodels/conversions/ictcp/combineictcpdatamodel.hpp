#pragma once


#include "datamodels/conversions/colcombinedatamodel.hpp"
#include "external/nodeeditor/include/QtNodes/NodeInfo.hpp"

namespace nitro {

    class CombineICtCpDataModel : public ColorCombineDataModel {
    public:
        explicit CombineICtCpDataModel();

        ~CombineICtCpDataModel() override = default;

        static QtNodes::NodeInfo nodeInfo() {
            return {"Combine ICtCp",
                    "CombineICtCp",
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
