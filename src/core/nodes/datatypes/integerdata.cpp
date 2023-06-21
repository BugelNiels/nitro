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

    void IntegerData::registerConversions() {

        // Every type needs a "conversion" to itself
        IntegerData::registerConversionFrom(IntegerData::id(),
                                            [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                                                return std::static_pointer_cast<IntegerData>(nodeData)->data();
                                            });

        IntegerData::registerConversionFrom(DecimalData::id(),
                                            [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                                                auto doubleData = std::static_pointer_cast<DecimalData>(nodeData);
                                                return int(std::round(doubleData->data()));
                                            });
    }


} // nitro
