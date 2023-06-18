#include "kmeans.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>

#define INPUT_IMAGE "Image"
#define INPUT_K "K"
#define OUTPUT_IMAGE "Image"

static cv::Mat kMeansColors(const cv::Mat &image, int numColors) {
    cv::Mat labels;
    cv::Mat centers;
    cv::Mat samples = image.reshape(1, image.rows * image.cols);
    cv::kmeans(samples,
               numColors,
               labels,
               cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 10, 0.001),
               1,
               cv::KMEANS_RANDOM_CENTERS, centers);

    cv::Mat quantImg(image.size(), image.type());
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            int cluster_idx = labels.at<int>(y * image.cols + x, 0);
            if (centers.cols == 3) {
                for (int i = 0; i < centers.cols; i++) {
                    float v = centers.at<float>(cluster_idx, i);
                    quantImg.at<cv::Vec3f>(y, x)[i] = v; // This is an out of bounds memory access?
                }
            } else {
                quantImg.at<float>(y, x) = centers.at<float>(cluster_idx, 0);
            }
        }
    }
    return quantImg;
}

void nitro::KMeansOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (!nodePorts.inputsPresent({INPUT_IMAGE, INPUT_K})) {
        return;
    }
    auto inputImg = nodePorts.getInputImage(INPUT_IMAGE);
    int k = nodePorts.getInputInteger(INPUT_K);

    cv::Mat kMeansDat = kMeansColors(*inputImg, k);

    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(kMeansDat));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::KMeansOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("K-Means", "kMeans", category);
        return builder.
                withOperator(std::make_unique<nitro::KMeansOperator>())->
                withIcon("quantize.png")->
                withNodeColor({43, 101, 43})->
                withInputImage(INPUT_IMAGE)->
                withInputInteger(INPUT_K, 8, 2, 255)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
