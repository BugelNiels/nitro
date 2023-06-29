#include "resample.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "util.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include "nodes/restoration/resampler/sampler.hpp"
#include "nodes/restoration/resampler/cubicsampler.hpp"
#include "nodes/restoration/resampler/smoothstepsampler.hpp"
#include "nodes/restoration/resampler/easeinsampler.hpp"
#include "nodes/restoration/resampler/easeoutsampler.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>

#define INPUT_IMAGE "Image"
#define INPUT_BITS "Bits"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"
#define OPTION_BRIGHTNESS_CORRECT "Correct Luminance"

static std::vector<float> getUniqueColors(const cv::Mat &src) {
    std::vector<float> uniqueColors;
    std::set<float> uniqueColorsSet(src.begin<float>(), src.end<float>());
    uniqueColors.assign(uniqueColorsSet.begin(), uniqueColorsSet.end());
    return uniqueColors;
}

// TODO: extract mats to reuse the buffers
static cv::Mat distanceField(const cv::Mat &src, float t) {

    int thresh = std::max(int((t * 255)) - 1, 0);
    cv::Mat binaryIm;
    cv::threshold(src, binaryIm, thresh, 255, cv::THRESH_BINARY_INV);

    // Calculate the distance transform
    cv::Mat df;
    cv::distanceTransform(binaryIm, df, cv::DIST_L2, cv::DIST_MASK_PRECISE);
    cv::Mat dfIn;
    cv::threshold(src, binaryIm, thresh, 255, cv::THRESH_BINARY);
    cv::distanceTransform(binaryIm, dfIn, cv::DIST_L2, cv::DIST_MASK_PRECISE);

    cv::subtract(df, dfIn, df);

    return df;
}

static std::vector<cv::Mat> getDfs(const cv::Mat &src, const std::vector<float> &colTable, int numLevels) {
    std::vector<cv::Mat> df(numLevels);

    cv::Mat grayImage;
    src.convertTo(grayImage, CV_8U, 255);

#pragma omp parallel for default(none) shared(df, grayImage, colTable) firstprivate(numLevels)
    for (int d = 0; d < numLevels; d++) {
        df[d] = distanceField(grayImage, colTable[d]);
    }

//    if (!cv::norm(df[1], df[0], cv::NORM_L1)) {
//        df[0] -= 10;
//    }
    return df;
}

cv::Mat nitro::resampleImage(const cv::Mat &img, int mode, int bits, bool brightnessCorrect) {
    std::unique_ptr<Sampler> sampler;
    switch (mode) {
        case 0:
            sampler = std::make_unique<Sampler>();
            break;
        case 1:
            sampler = std::make_unique<SmoothSampler>();
            break;
        case 2:
            sampler = std::make_unique<EaseInSampler>();
            break;
        case 3:
            sampler = std::make_unique<EaseOutSampler>();
            break;
        case 4:
            sampler = std::make_unique<CubicSampler>();
            break;
        default:
            sampler = std::make_unique<Sampler>();
            break;
    }
    auto colTable = getUniqueColors(img);
    int numLevels = colTable.size();
    std::vector<cv::Mat> dfs = getDfs(img, colTable, numLevels);
    float brightness = std::min(colTable[colTable.size() - 1] + 0.1f, 1.0f);
    colTable.push_back(brightness);
    dfs.push_back(dfs[dfs.size() - 1] + 100);

    int numDesiredLevels = int(std::pow(2, bits));
    cv::Mat result = sampler->resample(img, colTable, dfs, numDesiredLevels);
    if (brightnessCorrect) {
        cv::Mat resIn;
        cv::GaussianBlur(result, resIn, cv::Size(33, 33), 32, 32, cv::BorderTypes::BORDER_REFLECT);
        cv::Mat resTarget;
        cv::GaussianBlur(img, resTarget, cv::Size(33, 33), 32, 32, cv::BorderTypes::BORDER_REFLECT);
        cv::subtract(resIn, resTarget, resTarget);
        cv::subtract(result, resTarget, result);
    }
    return result;
}

void nitro::ResampleOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    int bits = nodePorts.inputInteger(INPUT_BITS);
    auto imIn = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    bool correctLum = nodePorts.optionEnabled(OPTION_BRIGHTNESS_CORRECT);

    int mode = nodePorts.getOption(MODE_DROPDOWN);
    cv::Mat result = resampleImage(*imIn, mode, bits, correctLum);
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ResampleOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Resample", "resample", category);
        return builder.
                withOperator(std::make_unique<nitro::ResampleOperator>())->
                withIcon("resample.png")->
                withNodeColor(NITRO_RESTORATION_COLOR)->
                withDropDown(MODE_DROPDOWN, {"Linear", "Smooth", "Ease In", "Ease Out", "Cubic"})->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withCheckBox(OPTION_BRIGHTNESS_CORRECT, true)->
                withInputInteger(INPUT_BITS, 8, 1, 16)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}
