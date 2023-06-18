#pragma once

#include <utility>

#include "QtNodes/NodeData"
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

        [[nodiscard]] QtNodes::NodeDataType type() const override {
            return QtNodes::NodeDataType{dataInfo().getDataId(), dataInfo().getDataName()};
        }

        [[nodiscard]] int value() const { return val_; }

        [[nodiscard]] QString getDescription() const override  {
            return QString::number(val_);
        }

    private:
        int val_ = 0;
    };
} // nitro
