#include "distancetransform.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_THRESH "Threshold"
#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"
#define OPTION_SIGNED "Signed"

static cv::Mat distanceField(cv::Mat const &src, double threshold, cv::DistanceTypes mode, bool signedDf) {
    cv::Mat binaryImOutside;
    cv::threshold(src, binaryImOutside, threshold, 1, cv::THRESH_BINARY_INV);
    binaryImOutside.convertTo(binaryImOutside, CV_8UC1, 255);

    cv::Mat df;
    cv::distanceTransform(binaryImOutside, df, mode, cv::DIST_MASK_PRECISE);
    cv::normalize(df, df, 0, 1, cv::NORM_MINMAX);
    if (signedDf) {
        cv::Mat binaryImInside;
        cv::threshold(src, binaryImInside, threshold, 1, cv::THRESH_BINARY);
        binaryImInside.convertTo(binaryImInside, CV_8UC1, 255);
        cv::Mat dfIn;
        cv::distanceTransform(binaryImInside, dfIn, mode, cv::DIST_MASK_PRECISE);
        cv::normalize(dfIn, dfIn, 0, 1, cv::NORM_MINMAX);
        cv::subtract(df, dfIn, df);
    }
    df.convertTo(df, CV_32F);
    return df;
}


void nitro::DistanceTransformOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    // Get the input data
    auto inputImg = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);;
    double threshold = nodePorts.inputValue(INPUT_THRESH);
    int mode = nodePorts.getOption(MODE_DROPDOWN);
    int signedDf = nodePorts.getOption(OPTION_SIGNED);

    cv::DistanceTypes type;
    switch (mode) {
        case 0:
            type = cv::DIST_L2;
            break;
        case 1:
            type = cv::DIST_L1;
            break;
        case 2:
            type = cv::DIST_C;
            break;
        case 3:
            type = cv::DIST_FAIR;
            break;
        case 4:
            type = cv::DIST_WELSCH;
            break;
        case 5:
            type = cv::DIST_HUBER;
            break;
        default:
            type = cv::DIST_L1;

    }

    cv::Mat result = distanceField(*inputImg, threshold, type, signedDf);
    // Store the result
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::DistanceTransformOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Distance Transform", "distanceTransform", category);
        return builder.
                withOperator(std::make_unique<nitro::DistanceTransformOperator>())->
                withIcon("distance.png")->
                withNodeColor(NITRO_ANALYSIS_COLOR)->
                withInputValue(INPUT_THRESH, 0.5, 0, 1)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withDropDown(MODE_DROPDOWN, {"Euclidean", "Manhattan", "Chessboard"})->
                withCheckBox(OPTION_SIGNED, false)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}
