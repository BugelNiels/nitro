#include "kmeans.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>

cv::Mat quantizeColors(const cv::Mat &image, int numColors) {
    cv::Mat reshapedImage = image.reshape(1, image.rows * image.cols);

    cv::Mat reshapedImageFloat;
    reshapedImage.convertTo(reshapedImageFloat, CV_32F);

    cv::Mat labels, centers;
    cv::kmeans(reshapedImageFloat, numColors, labels,
               cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 10, 1.0), 1,
               cv::KMEANS_RANDOM_CENTERS, centers);

    cv::Mat centers8u;
    centers.convertTo(centers8u, CV_8U);

    cv::Mat quantizedImage = centers8u.reshape(3, image.rows);

    quantizedImage = quantizedImage.reshape(3, image.rows);

    return quantizedImage;
}

void nitro::KMeansOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    bool kPresent, imPresent;
    int k = nodePorts.getInputInteger("k", kPresent);
    auto inputImg = nodePorts.getInputImage("image", imPresent);

    if (!kPresent || !imPresent) {
        return;
    }

    cv::Mat kMeansDat = quantizeColors(*inputImg, k);

    nodePorts.setOutputImage("image", std::make_shared<cv::Mat>(kMeansDat));
}
