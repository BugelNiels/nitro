#include "kmeans.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>

static cv::Mat kMeansColors(const cv::Mat &image, int numColors) {

    cv::Mat samples(image.rows * image.cols, image.channels(), CV_32F);
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            for (int z = 0; z < image.channels(); z++) {
                if (image.channels() == 3) {
                    samples.at<float>(y * image.cols + x, z) = image.at<cv::Vec3b>(y, x)[z];
                } else {
                    samples.at<float>(y * image.cols + x, z) = image.at<uchar>(y, x);
                }
            }
        }
    }

    cv::Mat labels, centers;
    cv::kmeans(samples, numColors, labels,
               cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 10, 1.0), 1,
               cv::KMEANS_RANDOM_CENTERS, centers);

    cv::Mat quantImg(image.size(), image.type());
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            int cluster_idx = labels.at<int>(y * image.cols + x, 0);
            if (image.channels() == 3) {
                for (int i = 0; i < image.channels(); i++) {
                    quantImg.at<cv::Vec3b>(y, x)[i] = centers.at<float>(cluster_idx, i);
                }
            } else {
                quantImg.at<uchar>(y, x) = centers.at<float>(cluster_idx, 0);
            }
        }
    }
    return quantImg;
}

void nitro::KMeansOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    bool kPresent, imPresent;
    int k = nodePorts.getInputInteger("K", kPresent);
    auto inputImg = nodePorts.getInputImage("Image", imPresent);

    if (!kPresent || !imPresent) {
        return;
    }

    cv::Mat kMeansDat = kMeansColors(*inputImg, k);

    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(kMeansDat));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::KMeansOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("K-Means", "kMeans", category);
        return builder.
                withOperator(std::make_unique<nitro::KMeansOperator>())->
                withIcon(":/icons/nodes/quantize.png")->
                withNodeColor({43, 101, 43})->
                withInputImage("Image")->
                withInputInteger("K", 8, 1, 255)->
                withOutputImage("Image")->
                build();
    };
}
