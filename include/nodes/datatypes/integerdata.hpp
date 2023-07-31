#pragma once

#include <utility>

#include "flexibledata.hpp"
#include <QtNodes/NodeData>

namespace nitro {

/**
 * @brief Describes an integer data type.
 */
class IntegerData : public FlexibleData<int, IntegerData> {
public:
    /**
     * @brief Creates a new empty integer data type.
     */
    IntegerData();

    /**
     * @brief Creates a integer data type with the provided value.
     * @param value The value this type should have.
     */
    explicit IntegerData(int value);

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
    inline static const QString name_ = "Integer";
    inline static const QString id_ = "Integer";
    inline static const QColor baseColor_ = {89, 140, 92};
};

} // namespace nitro
