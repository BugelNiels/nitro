#pragma once

#include <nodes/nodeoperator.hpp>
#include "gui/imviewer/imviewer.hpp"
#include <nodes/nitronode.hpp>

namespace nitro::ImCore {

class ValueViewOperator : public NodeOperator {
public:

    explicit ValueViewOperator(QLabel *valueLabel);

    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *valueLabel_;
};

} // namespace nitro::ImCore
