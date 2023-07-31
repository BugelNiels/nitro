#pragma once

#include "flexibledata.hpp"
#include <QtNodes/NodeData>

namespace nitro {

/**
 * @brief Describes a decimal data type.
 */
class DecimalData : public FlexibleData<double, DecimalData> {
public:
    /**
     * @brief Creates a new empty decimal data type.
     */
    DecimalData();

    /**
     * @brief Creates a decimal data type with the provided value.
     * @param value The value this type should have.
     */
    explicit DecimalData(double value);

    /**
     * @brief Returns the unique id of this data type.
     * @return The unique id of this data type.
     */
    static QString id() { return id_; }

    /**
     * @brief Registers the conversions that this data type allows.
     */
    static void registerConversions();

    /**
     * @brief Retrieves the description of this data type.
     * @return The description of this data type.
     */
    [[nodiscard]] QString getDescription() const override;

private:
    inline static const QString name_ = "Decimal";
    inline static const QString id_ = "Decimal";
    inline static const QColor baseColor_ = {161, 161, 161};
};

} // namespace nitro
