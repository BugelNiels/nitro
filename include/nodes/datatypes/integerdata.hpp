#pragma once

#include <utility>

#include "QtNodes/NodeData"
#include "flexibledata.hpp"

namespace nitro {
    class IntegerData : public FlexibleData<int> {
    public:
        IntegerData();

        explicit IntegerData(int value);

        // TODO: check if we can do without this being static
        static QString id() {
            return id_;
        }

        [[nodiscard]] QString getDescription() const override;

    private:
        inline static const QString name_ = "Integer";
        inline static const QString id_ = "Integer";
        inline static const QColor baseColor_ = {89, 140, 92};
    };
} // nitro
