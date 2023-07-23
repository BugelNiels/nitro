#pragma once

#include <nodes/nitronode.hpp>
#include <nodes/nodeoperator.hpp>

namespace nitro::Thesis {

class CompressOperator : public NodeOperator {
public:
    explicit CompressOperator(QLabel *timeLabel);

    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *timeLabel_;
};

} // namespace nitro::Thesis
