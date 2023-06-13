#pragma once

#include <QImage>
#include <opencv2/core/mat.hpp>

namespace nitro {

    class Sampler {
    public:
        Sampler();

        virtual ~Sampler();

        virtual cv::Mat resample(const cv::Mat &colTable,
                                 const std::vector<cv::Mat> &df,
                                 int numDesiredLevels);

    protected:

        virtual float distFuncIndexed(const cv::Mat &colorTable,
                                      const std::vector<cv::Mat> &df,
                                      int x, int y, float p, int numLevelsInput) const = 0;
    };
}  // namespace nitro
