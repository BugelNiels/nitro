#pragma once

#include <utility>

#include "QtNodes/NodeData"
#include "nodes/datainfo.hpp"
#include "flexibledata.hpp"
#include "imagedata.hpp"

namespace nitro {
    class DecimalData : public FlexibleData<double> {
    public:
        DecimalData() : FlexibleData<double>(0, dataInfo(), baseColor_) {}

        explicit DecimalData(double value) : FlexibleData<double>(value, dataInfo(), baseColor_) {}

        static nitro::DataInfo dataInfo() {
            return {name_, id_, baseColor_};
        }

        [[nodiscard]] double value() const { return data(); }

        [[nodiscard]] QString getDescription() const override {
            return QString::number(data(), 'f', 3);
        }

    private:
        inline static const QString name_ = "Double";
        inline static const QString id_ = "Decimal";
        inline static const QColor baseColor_ = {161, 161, 161};
    };
} // nitro
