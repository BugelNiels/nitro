#pragma once

#include "sampler.hpp"

namespace nitro {

    class LinearSampler : public Sampler {
    public:
        LinearSampler();

        ~LinearSampler() override;

    protected:
        [[nodiscard]] float distFuncIndexed(const cv::Mat &colorTable,
                              const std::vector<cv::Mat> &df,
                              int x, int y, float p, int numLevelsInput) const override;
    };

}  // namespace nitro
