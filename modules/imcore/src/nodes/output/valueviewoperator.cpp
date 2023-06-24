#include "valueviewoperator.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/decimaldata.hpp"

#define DISPLAY_LABEL "valueLabel"
#define INPUT_VALUE "Value"

#include <QDebug>


nitro::ValueViewOperator::ValueViewOperator(QLabel *valueLabel) : valueLabel_(valueLabel) {}

void nitro::ValueViewOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    double val = nodePorts.inGetAs<DecimalData>(INPUT_VALUE);
    double intPart;
    if (std::modf(val, &intPart) == 0.0) {
        valueLabel_->setText(QString::number(val));
    } else {
        valueLabel_->setText(QString::number(val, 'f', 3));
    }
}

std::function<std::unique_ptr<nitro::NitroNode>()>
nitro::ValueViewOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Value Display", "valueViewer", category);
        auto *valueLabel = new QLabel("-");
        return builder.
                withOperator(std::make_unique<nitro::ValueViewOperator>(valueLabel))->
                withIcon("number.png")->
                withDisplayWidget(DISPLAY_LABEL, valueLabel)->
                withNodeColor(NITRO_OUTPUT_COLOR)->
                withInputPort<DecimalData>(INPUT_VALUE)->
                build();
    };
}

