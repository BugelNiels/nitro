#pragma once

#include <nodes/nitronode.hpp>
#include <nodes/nodeoperator.hpp>

namespace nitro::Compression {

class PngOperator : public NodeOperator {
public:
    explicit PngOperator(QLabel *valueLabel, QLabel *originalSizeLabel, QLabel *ratioLabel);

    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *valueLabel_;
    QLabel *originalSizeLabel_;
    QLabel *ratioLabel_;
};

} // namespace nitro::Compression
