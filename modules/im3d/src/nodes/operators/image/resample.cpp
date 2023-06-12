#include "resample.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/operators/image/resampler/resampler.hpp"
#include "nodes/operators/image/resampler/linearresampler.hpp"
#include "nodes/operators/image/resampler/cubicinterpolatoryresampler.hpp"
#include "util.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>
#include <opencv2/highgui.hpp>

static cv::Mat getUniqueColors(const cv::Mat &src) {

    cv::Mat reshapedImage = src.reshape(1, 1);

    std::vector<int> uniqueColors;
    std::set<int> uniqueColorsSet(reshapedImage.begin<uchar>(), reshapedImage.end<uchar>());
    uniqueColors.assign(uniqueColorsSet.begin(), uniqueColorsSet.end());

    cv::Mat colTable;
    colTable.create(static_cast<int>(uniqueColors.size()), 1, CV_8U);
    for (size_t i = 0; i < uniqueColors.size(); ++i) {
        colTable.at<uchar>(i) = uniqueColors[i];
    }
    return colTable;
}

cv::Mat distanceField(const cv::Mat &src, int t, int levels) {
    cv::Mat binaryImOutside, binaryImInside;
    cv::threshold(src, binaryImOutside, t, levels, cv::THRESH_BINARY_INV);
    cv::threshold(src, binaryImInside, t, levels, cv::THRESH_BINARY);

    // Calculate the distance transform
    cv::Mat dfIn, dfOut;
    cv::distanceTransform(binaryImInside, dfIn, cv::DIST_L2, cv::DIST_MASK_PRECISE);
    cv::distanceTransform(binaryImOutside, dfOut, cv::DIST_L2, cv::DIST_MASK_PRECISE);

    cv::Mat df;
    cv::subtract(dfOut, dfIn, df);

    return df;
}

std::vector<cv::Mat> getDfs(const cv::Mat &src, const cv::Mat &colTable, int numLevels) {
    std::vector<cv::Mat> df(numLevels);
    int maxInputLevels = nitro::getMaxValue(src);

    cv::Mat l0(src.rows, src.cols, CV_32FC1);
#pragma omp parallel for default(none) shared(df, src, colTable) firstprivate(numLevels, maxInputLevels)
    for (int d = 1; d < numLevels; d++) {
        int threshold = colTable.at<uchar>(d, 0);
        df[d] = distanceField(src, threshold, maxInputLevels);
    }

    if (numLevels > 1) {
        df[1].copyTo(df[0]);
        df[0] -= 10;
    } else {
        df[0] = distanceField(src, 1, numLevels);
    }
    return df;
}

void nitro::ResampleOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    bool kPresent, imPresent;
    int bits = nodePorts.getInputInteger("Bits", kPresent);
    auto inputImg = nodePorts.getInputImage("Image", imPresent);

    if (!kPresent || !imPresent) {
        return;
    }

    cv::Mat imIn;
    if (inputImg->channels() > 1) {
        cvtColor(*inputImg, imIn, cv::COLOR_RGB2GRAY);
    } else {
        imIn = *inputImg;
    }

    int mode = options.at("Mode");
    std::unique_ptr<Resampler> resampler;
    switch (mode) {
        case 0:
            resampler = std::make_unique<LinearSampler>();
            break;
        case 1:
            resampler = std::make_unique<CubicInterpolatorySampler>();
            break;
        default:
            resampler = std::make_unique<LinearSampler>();
            break;
    }
    cv::Mat colTable = getUniqueColors(imIn);
    int numLevels = colTable.rows;
    std::vector<cv::Mat> dfs = getDfs(imIn, colTable, numLevels);

    cv::Mat result = resampler->resample(colTable, dfs, std::pow(2, bits));

    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ResampleOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Resample", "resample", category);
        return builder.
                withOperator(std::make_unique<nitro::ResampleOperator>())->
                withIcon(":/icons/nodes/resample.png")->
                withNodeColor({201, 94, 6})->
                withDropDown("Mode", {"Linear", "Cubic"})->
                withInputImage("Image")->
                withInputInteger("Bits", 8, 16, 1)->
                withOutputImage("Image")->
                build();
    };
}
