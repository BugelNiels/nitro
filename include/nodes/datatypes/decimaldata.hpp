#pragma once

#include <utility>

#include "QtNodes/NodeData"
#include "QtNodes/DataInfo.hpp"

namespace nitro {
    class DecimalData : public QtNodes::NodeData {
    public:
        DecimalData() = default;

        explicit DecimalData(double val) : val_(val) {}

        static QtNodes::DataInfo dataInfo() {
            return {"Value", "decimal", {161, 161, 161}};
        }

        [[nodiscard]] QtNodes::NodeDataType type() const override {
            return QtNodes::NodeDataType{dataInfo().getDataId(), dataInfo().getDataName()};
        }

        [[nodiscard]] double value() const { return val_; }

        [[nodiscard]] QString getDescription() const override  {
            return QString::number(val_, 'f', 3);
        }

    private:
        double val_ = 0;
    };
} // nitro
