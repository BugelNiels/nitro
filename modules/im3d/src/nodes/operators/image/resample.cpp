#include "resample.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/operators/image/resampler/sampler.hpp"
#include "nodes/operators/image/resampler/linearsampler.hpp"
#include "nodes/operators/image/resampler/cubicsampler.hpp"
#include "util.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>

#define INPUT_IMAGE "Image"
#define INPUT_BITS "Bits"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

static cv::Mat getUniqueColors(const cv::Mat &src) {

    cv::Mat reshapedImage = src.reshape(1, 1);
    std::vector<float> uniqueColors;
    std::set<float> uniqueColorsSet(reshapedImage.begin<float>(), reshapedImage.end<float>());
    uniqueColors.assign(uniqueColorsSet.begin(), uniqueColorsSet.end());

    cv::Mat colTable;
    colTable.create(static_cast<int>(uniqueColors.size()), 1, CV_32FC1);
    for (int i = 0; i < uniqueColors.size(); ++i) {
        colTable.at<float>(i) = uniqueColors[i];
    }
    return colTable;
}

cv::Mat distanceField(const cv::Mat &src, float t) {

    int thresh = int(std::round(t * 255));
    cv::Mat binaryImOutside, binaryImInside;
    cv::threshold(src, binaryImOutside, thresh, 255, cv::THRESH_BINARY_INV);
    cv::threshold(src, binaryImInside, thresh, 255, cv::THRESH_BINARY);

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

    cv::Mat grayImage;
    src.convertTo(grayImage, CV_8U, 255);

#pragma omp parallel for default(none) shared(df, grayImage, colTable) firstprivate(numLevels, maxInputLevels)
    for (int d = 1; d < numLevels; d++) {
        float threshold = colTable.at<float>(d, 0);
        df[d] = distanceField(grayImage, threshold);
    }

    if (numLevels > 1) {
        df[1].copyTo(df[0]);
        df[0] -= 10;
    } else {
        df[0] = distanceField(grayImage, 1);
    }
    return df;
}

void nitro::ResampleOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (!nodePorts.inputsPresent({INPUT_IMAGE, INPUT_BITS})) {
        return;
    }
    int bits = nodePorts.getInputInteger(INPUT_BITS);
    auto inputImg = nodePorts.getInputImage(INPUT_IMAGE);

    cv::Mat imIn;
    if (inputImg->channels() > 1) {
        cvtColor(*inputImg, imIn, cv::COLOR_RGB2GRAY);
    } else {
        imIn = *inputImg;
    }

    int mode = options.at(MODE_DROPDOWN);
    std::unique_ptr<Sampler> sampler;
    switch (mode) {
        case 0:
            sampler = std::make_unique<LinearSampler>();
            break;
        case 1:
            sampler = std::make_unique<CubicSampler>();
            break;
        default:
            sampler = std::make_unique<LinearSampler>();
            break;
    }
    cv::Mat colTable = getUniqueColors(imIn);
    int numLevels = colTable.rows;
    std::vector<cv::Mat> dfs = getDfs(imIn, colTable, numLevels);

    cv::Mat result = sampler->resample(colTable, dfs, std::pow(2, bits));

    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ResampleOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Resample", "resample", category);
        return builder.
                withOperator(std::make_unique<nitro::ResampleOperator>())->
                withIcon("resample.png")->
                withNodeColor({201, 94, 6})->
                withDropDown(MODE_DROPDOWN, {"Linear", "Cubic"})->
                withInputImage(INPUT_IMAGE)->
                withInputInteger(INPUT_BITS, 8, 1, 16)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
