#pragma once

#include <utility>

#include "QtNodes/NodeData"
#include "flexibledata.hpp"
#include "../../../modules/imcore/include/colimagedata.hpp"
#include "include/valueslider.hpp"

namespace nitro {

class DecimalData : public FlexibleData<double, DecimalData> {
public:
    DecimalData();

    explicit DecimalData(double value);

    static QString id() {
        return id_;
    }

    static void registerConversions();

    [[nodiscard]] QString getDescription() const override;

private:
    inline static const QString name_ = "Decimal";
    inline static const QString id_ = "Decimal";
    inline static const QColor baseColor_ = {161, 161, 161};
};

} // namespace nitro
