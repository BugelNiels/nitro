#include "nodes/datatypes/integerdata.hpp"
#include "nodes/datatypes/decimaldata.hpp"

namespace nitro {
    IntegerData::IntegerData() : FlexibleData<int>(0, id_, name_, baseColor_) {
        allowConversionFrom(DecimalData::id());
    }


    IntegerData::IntegerData(int value) : FlexibleData<int>(value, id_, name_, baseColor_) {
        allowConversionFrom(DecimalData::id());
    }


    QString IntegerData::getDescription() const {
        return QString::number(data());
    }

} // nitro
