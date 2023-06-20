#pragma once

#include <utility>

#include "QtNodes/NodeData"
#include "nodes/datainfo.hpp"
#include "flexibledata.hpp"
#include "imagedata.hpp"

namespace nitro {
    class DecimalData : public FlexibleData<double> {
    public:
        DecimalData() = default;

        using FlexibleData::FlexibleData;

        static nitro::DataInfo dataInfo() {
            return {name_, id_, baseColor_};
        }

        [[nodiscard]] QtNodes::NodeDataType type() const override {
            return type_;
        }

        [[nodiscard]] double value() const { return data(); }

        [[nodiscard]] QString getDescription() const override {
            return QString::number(data(), 'f', 3);
        }

        void allowConversions(bool checked) override {
            if(checked) {
                type_.allowedConversion.insert(ImageData::dataInfo().getDataId());
            } else {
                type_.allowedConversion.clear();
            }
        }

    private:
        inline static const QString name_ = "Double";
        inline static const QString id_ = "Decimal";
        inline static const QColor baseColor_ = {161, 161, 161};

        QtNodes::NodeDataType type_ = {dataInfo().getDataId(), dataInfo().getDataName(), baseColor_};
    };
} // nitro
