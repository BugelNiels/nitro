#pragma once

#include <utility>

#include "QtNodes/NodeData"
#include "flexibledata.hpp"
#include "colimagedata.hpp"

namespace nitro {
    class DecimalData : public FlexibleData<double> {
    public:
        DecimalData();

        explicit DecimalData(double value);

        static QString id() {
            return id_;
        }

        [[nodiscard]] QString getDescription() const override;

    private:
        inline static const QString name_ = "Double";
        inline static const QString id_ = "Decimal";
        inline static const QColor baseColor_ = {161, 161, 161};
    };
} // nitro
