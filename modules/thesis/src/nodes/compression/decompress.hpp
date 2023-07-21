#pragma once

#include <nodes/nodeoperator.hpp>
#include <nodes/nitronode.hpp>

namespace nitro::Thesis {

class DecompressOperator : public NodeOperator {
public:
    explicit DecompressOperator(QLabel *timeLabel);

    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *timeLabel_;
};

} // namespace nitro::Thesis

