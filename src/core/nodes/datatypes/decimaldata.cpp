#include "nodes/datatypes/decimaldata.hpp"
#include "nodes/datatypes/integerdata.hpp"

namespace nitro {

    DecimalData::DecimalData() : FlexibleData<double, DecimalData>(0, id_, name_, baseColor_) {
        allowConversionFrom(IntegerData::id());
    }


    DecimalData::DecimalData(double value) : FlexibleData<double, DecimalData>(value, id_, name_, baseColor_) {
        allowConversionFrom(IntegerData::id());
    }


    QString DecimalData::getDescription() const {
        return QString::number(data(), 'f', 3);
    }

    void DecimalData::registerConversions() {

        // Every type needs a "conversion" to itself
        DecimalData::registerConversionFrom<DecimalData>([](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
            return std::static_pointer_cast<DecimalData>(nodeData)->data();
        });

        DecimalData::registerConversionFrom<IntegerData>([](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
            auto intData = std::static_pointer_cast<IntegerData>(nodeData);
            return double(intData->data());
        });
    }

} // nitro

