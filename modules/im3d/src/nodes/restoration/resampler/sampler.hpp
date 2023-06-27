#pragma once

#include <QImage>
#include <opencv2/core/mat.hpp>

namespace nitro {

    class Sampler {
    public:
        Sampler();

        virtual ~Sampler();

        virtual cv::Mat resample(const cv::Mat& img, const std::vector<float> &colTable,
                                 const std::vector<cv::Mat> &df,
                                 int numDesiredLevels);

    protected:
        [[nodiscard]] virtual float mix(float a, float b, float w) const;

        [[nodiscard]] float distFunc(const std::vector<float> &colorTable,
                                             const std::vector<cv::Mat> &df,
                                             int x, int y, float p, int numLevelsInput) const;
    };
}  // namespace nitro
