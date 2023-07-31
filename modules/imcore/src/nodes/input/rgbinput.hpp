#pragma once

#include <nodes/nitronode.hpp>
#include <nodes/nodeoperator.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

class RgbOperator : public NodeOperator {

public:
    explicit RgbOperator(QLabel *colLabel);

    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *colLabel_;
    QPixmap colLabelPixMap_;
};

} // namespace nitro::ImCore
