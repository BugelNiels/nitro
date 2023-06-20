#pragma once

#include <utility>

#include "QtNodes/NodeData"
#include "nodes/datainfo.hpp"
#include "flexibledata.hpp"

namespace nitro {
    class IntegerData : public FlexibleData<int> {
    public:
        IntegerData() = default;

        using FlexibleData::FlexibleData;

        // TODO: check if we can do without this being static
        static nitro::DataInfo dataInfo() {
            return {name_, "Integer", baseColor_};
        }

        [[nodiscard]] QtNodes::NodeDataType type() const override {
            return QtNodes::NodeDataType{dataInfo().getDataId(), dataInfo().getDataName(), baseColor_};
        }

        [[nodiscard]] int value() const { return data(); }

        [[nodiscard]] QString getDescription() const override {
            return QString::number(data());
        }

    private:
        inline static const QString name_ = "Integer";
        inline static const QColor baseColor_ = {89, 140, 92};
    };
} // nitro
