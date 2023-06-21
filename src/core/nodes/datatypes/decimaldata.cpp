#include "nodes/datatypes/decimaldata.hpp"
#include "nodes/datatypes/integerdata.hpp"

namespace nitro {
    DecimalData::DecimalData() : FlexibleData<double>(0, id_, name_, baseColor_) {
        allowConversionFrom(IntegerData::id());
    }


    DecimalData::DecimalData(double value) : FlexibleData<double>(value, id_, name_, baseColor_) {
        allowConversionFrom(IntegerData::id());
    }


    QString DecimalData::getDescription() const {
        return QString::number(data(), 'f', 3);
    }

} // nitro
