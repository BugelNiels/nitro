#pragma once

#include <utility>

#include "flexibledata.hpp"
#include <QtNodes/NodeData>

namespace nitro {

class IntegerData : public FlexibleData<int, IntegerData> {
public:
    IntegerData();

    explicit IntegerData(int value);

    static QString id() { return id_; }

    static void registerConversions();

    [[nodiscard]] QString getDescription() const override;

private:
    inline static const QString name_ = "Integer";
    inline static const QString id_ = "Integer";
    inline static const QColor baseColor_ = {89, 140, 92};
};

} // namespace nitro
