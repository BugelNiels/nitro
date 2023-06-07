#pragma once

#include <utility>

#include "QtNodes/NodeData"

#include "src/core/cbdimage.hpp"
#include "QtNodes/DataInfo.hpp"

namespace nitro {
    class IntegerData : public QtNodes::NodeData {
    public:
        IntegerData() = default;

        explicit IntegerData(int val) : val_(val) {
        }

        static QtNodes::DataInfo dataInfo() {
            return {"Value", "integer", {89, 140, 92}};
        }

        QtNodes::NodeDataType type() const override {
            return QtNodes::NodeDataType{dataInfo().getDataId(), dataInfo().getDataName()};
        }

        int value() const { return val_; }

    private:
        int val_;
    };
} // nitro