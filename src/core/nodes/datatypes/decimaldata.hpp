#pragma once

#include <utility>

#include "external/nodeeditor/include/QtNodes/NodeData"


#include "src/core/cbdimage.hpp"
#include "external/nodeeditor/include/QtNodes/DataInfo.hpp"

namespace nitro {
    class DecimalData : public QtNodes::NodeData {
    public:
        DecimalData() = default;

        explicit DecimalData(double val) : val_(val) {
        }


        static QtNodes::DataInfo dataInfo() {
            return {"Value", "decimal", {161, 161, 161}};
        }

        QtNodes::NodeDataType type() const override {
            return QtNodes::NodeDataType{dataInfo().getDataId(), dataInfo().getDataName()};
        }

        double value() const { return val_; }

    private:
        double val_;
    };
} // nitro