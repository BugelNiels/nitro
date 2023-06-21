#pragma once

#include <utility>

#include "QtNodes/NodeData"
#include "nodes/datainfo.hpp"
#include "flexibledata.hpp"

namespace nitro {
    class IntegerData : public FlexibleData<int> {
    public:
        IntegerData() : FlexibleData<int>(0, dataInfo(), baseColor_) {}

        explicit IntegerData(int value) : FlexibleData<int>(value, dataInfo(), baseColor_) {}

        // TODO: check if we can do without this being static
        static nitro::DataInfo dataInfo() {
            return {name_, id_, baseColor_};
        }

        [[nodiscard]] int value() const { return data(); }

        [[nodiscard]] QString getDescription() const override {
            return QString::number(data());
        }

    private:
        inline static const QString name_ = "Integer";
        inline static const QString id_ = "Integer";
        inline static const QColor baseColor_ = {89, 140, 92};
    };
} // nitro
