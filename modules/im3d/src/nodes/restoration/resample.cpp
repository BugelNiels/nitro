#include "resample.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "util.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include "nodes/restoration/resampler/sampler.hpp"
#include "nodes/restoration/resampler/cubicsampler.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>

#define INPUT_IMAGE "Image"
#define INPUT_BITS "Bits"
#define INPUT_LOWER_OFFSET "Offset L0"
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

static cv::Mat getNextLabels(const cv::Mat &src, const cv::Mat &colTable, int numDesiredLevels) {
    cv::Mat nextLabels(src.rows, src.cols, CV_32SC1);
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            float val = src.at<float>(y, x);
            int next = numDesiredLevels - 1;
            for (int i = 0; i < colTable.rows - 1; ++i) {
                if (val == colTable.at<float>(i)) {
                    next = colTable.at<float>(i + 1) * (numDesiredLevels - 1.0f);
                    break;
                }
            }
            nextLabels.at<int>(y, x) = next;
        }
    }
    return nextLabels;
}

// TODO: extract mats to reuse the buffers
cv::Mat distanceField(const cv::Mat &src, float t) {

    int thresh = int((t * 255)) - 1;
    cv::Mat binaryImOutside;
    cv::Mat binaryImInside;
    cv::threshold(src, binaryImOutside, thresh, 255, cv::THRESH_BINARY_INV);
    cv::threshold(src, binaryImInside, thresh, 255, cv::THRESH_BINARY);

    // Calculate the distance transform
    cv::Mat dfIn;
    cv::Mat df;
    cv::distanceTransform(binaryImInside, dfIn, cv::DIST_L2, cv::DIST_MASK_PRECISE);
    cv::distanceTransform(binaryImOutside, df, cv::DIST_L2, cv::DIST_MASK_PRECISE);

    cv::subtract(df, dfIn, df);

    return df;
}

std::vector<cv::Mat> getDfs(const cv::Mat &src, const cv::Mat &colTable, int numLevels, int offset) {
    std::vector<cv::Mat> df(numLevels);

    cv::Mat grayImage;
    src.convertTo(grayImage, CV_8U, 255);

#pragma omp parallel for default(none) shared(df, grayImage, colTable) firstprivate(numLevels)
    for (int d = 1; d < numLevels; d++) {
        float threshold = colTable.at<float>(d, 0);
        df[d] = distanceField(grayImage, threshold);
    }

    if (numLevels > 1) {
        df[1].copyTo(df[0]);
        df[0] -= offset;
    } else {
        df[0] = distanceField(grayImage, 1);
    }
    return df;
}

void nitro::ResampleOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    int bits = nodePorts.inputInteger(INPUT_BITS);
    auto imIn = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    int offset = nodePorts.inputInteger(INPUT_LOWER_OFFSET);

    int mode = nodePorts.getOption(MODE_DROPDOWN);
    std::unique_ptr<Sampler> sampler;
    switch (mode) {
        case 0:
            sampler = std::make_unique<Sampler>();
            break;
        case 1:
            sampler = std::make_unique<CubicSampler>();
            break;
        default:
            sampler = std::make_unique<Sampler>();
            break;
    }
    cv::Mat colTable = getUniqueColors(*imIn);
    int numLevels = colTable.rows;
    std::vector<cv::Mat> dfs = getDfs(*imIn, colTable, numLevels, offset);

    int numDesiredLevels = int(std::pow(2, bits));
    cv::Mat nextLabels = getNextLabels(*imIn, colTable, numDesiredLevels);
    cv::Mat result = sampler->resample(nextLabels, colTable, dfs, int(std::pow(2, bits)));

    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ResampleOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Resample", "resample", category);
        return builder.
                withOperator(std::make_unique<nitro::ResampleOperator>())->
                withIcon("resample.png")->
                withNodeColor(NITRO_RESTORATION_COLOR)->
                withDropDown(MODE_DROPDOWN, {"Linear", "Cubic"})->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_BITS, 8, 1, 16)->
                withInputInteger(INPUT_LOWER_OFFSET, 10, 0, 100)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}
