#pragma once

#include "src/improc/core/cbdimage.hpp"
#include "core/algorithms/nodealgorithm.hpp"

namespace nitro::operations {

    CbdImage threshold(const CbdImage &inputImg, int threshold, bool greater = false);

class ThresholdAlgorithm : public nitro::NodeAlgorithm {

    void compute(const std::map<QString, std::shared_ptr<QtNodes::NodeData>> &input,
                         std::map<QString, std::shared_ptr<QtNodes::NodeData>> &output,
                         const std::map<QString, QString> &options) const override;

};

} // nitro::operations
