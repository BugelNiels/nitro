#pragma once

#include "sampler.hpp"

namespace nitro {

    class CubicSampler : public Sampler {
    public:
        CubicSampler();

        ~CubicSampler() override;


        cv::Mat resample(const cv::Mat &colTable,
                         const std::vector<cv::Mat> &df,
                         int numDesiredLevels) override;

    };

}  // namespace nitro
