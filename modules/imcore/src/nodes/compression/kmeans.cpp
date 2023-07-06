#include "kmeans.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>

#define INPUT_IMAGE "Image"
#define INPUT_K "K"
#define INPUT_MAX_ITER "Max Iter"
#define OUTPUT_IMAGE "Image"


#define NUM_BINS 256

static inline int findClosestMean(const std::vector<float> &means, int k, int val) {
    int meanIdx = 0;
    float curDistance = std::abs(val - means[0]);
    for (int i = 1; i < k; i++) {
        float dist = std::abs(val - means[i]);
        if (dist < curDistance) {
            curDistance = dist;
            meanIdx = i;
        }
    }
    return meanIdx;
}

static void histKMeans(const std::vector<int> &hist, int k, int iter, double epsilon, std::vector<float> &means,
                       std::vector<int> &labels) {
    int size = hist.size();

    means.resize(k);
    std::vector<int> meanCounts(k);
    std::vector<float> oldMeans(k);
    // TODO: epsilon meanDiff vec
    std::fill(meanCounts.begin(), meanCounts.end(), 0);
    std::fill(oldMeans.begin(), oldMeans.end(), 0);
    for (int i = 0; i < k; i++) {
        means[i] = i * 255.0f / (k - 1.0);
    }

    labels.resize(size);

    for (int i = 0; i < iter; i++) {

        for (int j = 0; j < size; ++j) {
            int closestMeanIdx = findClosestMean(means, k, j);
            labels[j] = closestMeanIdx;
            meanCounts[closestMeanIdx] += hist[j];
        }
        // Update means
        std::fill(means.begin(), means.end(), 0);
        for (int j = 0; j < size; ++j) {
            means[labels[j]] += j * hist[j];

        }
        float diff = 0;
        for (int j = 0; j < k; j++) {
            if (meanCounts[j] == 0) {
                continue;
            }
            means[j] /= float(meanCounts[j]);
            diff += std::abs(means[j] - oldMeans[j]);
            meanCounts[j] = 0;
            oldMeans[j] = means[j];
        }
        if (diff < epsilon) {
            break;
        }
    }
}

static std::vector<int> getHistogram(const cv::Mat &src) {
    std::vector<int> hist(NUM_BINS);
    std::fill(hist.begin(), hist.end(), 0);
    for (int i = 0; i < src.rows; ++i) {
        const uchar *row_ptr = src.ptr<uchar>(i);
        for (int j = 0; j < src.cols; ++j) {
            hist[row_ptr[j]]++;
        }
    }
    return hist;
}

cv::Mat nitro::kMeansHist(const cv::Mat &image, int numColors, int maxIter) {

    cv::Mat img;
    image.convertTo(img, CV_8UC1, 255.0);
    auto hist = getHistogram(img);

    std::vector<float> centers;
    std::vector<int> labels;
    histKMeans(hist, numColors, maxIter, 0.01, centers, labels);

    // create LUT
    cv::Mat lut(1, NUM_BINS, CV_32FC1);
    float *lutRow = lut.ptr<float>();
    for (int i = 0; i < NUM_BINS; i++) {
        lutRow[i] = centers[labels[i]] / 255.0f;
    }

    cv::Mat result;
    cv::LUT(img, lut, result);
    return result;
}

static cv::Mat kMeansColors(const cv::Mat &image, int numColors, int maxIter, double epsilon, int maxAttempts) {
    cv::Mat labels;
    cv::Mat centers;
    cv::Mat samples = image.reshape(1, image.rows * image.cols);
    if (samples.rows < numColors) {
        return {};
    }
    cv::kmeans(samples,
               numColors,
               labels,
               cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, maxIter, epsilon),
               maxAttempts,
               cv::KMEANS_PP_CENTERS, centers);

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

void nitro::KMeansOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    int k = nodePorts.inputInteger(INPUT_K);
    int maxIter = nodePorts.inputInteger(INPUT_MAX_ITER);

    cv::Mat inputImg;
    cv::Mat kMeansDat;
    if (nodePorts.inputOfType<GrayImageData>(INPUT_IMAGE)) {
        inputImg = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
        kMeansDat = kMeansHist(inputImg, k, maxIter);
    } else {
        inputImg = *nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
        kMeansDat = kMeansColors(inputImg, k, maxIter, 0.00005, 1);
    }
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, kMeansDat);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::KMeansOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("K-Means", "kMeans", category);
        return builder.
                withOperator(std::make_unique<nitro::KMeansOperator>())->
                withIcon("quantize.png")->
                withNodeColor(NITRO_COMPRESSION_COLOR)->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_MAX_ITER, 20, 1, 50, BoundMode::LOWER_ONLY)->
                withInputInteger(INPUT_K, 8, 2, 255)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}
