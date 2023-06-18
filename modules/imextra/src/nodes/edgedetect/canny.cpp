#include "canny.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_THRESH_1 "Threshold 1"
#define INPUT_THRESH_2 "Threshold 2"
#define INPUT_APERTURE "Aperture"
#define OUTPUT_IMAGE "Image"

void nitro::CannyEdgeDetectionOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    // Verifying that all the required inputs are there
    if (!nodePorts.inputsPresent({INPUT_IMAGE, INPUT_THRESH_1, INPUT_THRESH_2, INPUT_APERTURE})) {
        return;
    }
    // Get the input data
    auto inputImg = nodePorts.getInputImage(INPUT_IMAGE);
    double thresh1 = nodePorts.getInputValue(INPUT_THRESH_1);
    double thresh2 = nodePorts.getInputValue(INPUT_THRESH_2);
    int aperture = nodePorts.getInputInteger(INPUT_APERTURE);
    if (aperture % 2 == 0) {
        aperture -= 1;
    }


    cv::Mat intImage;
    inputImg->convertTo(intImage, CV_8U, 255);

    // Perform filtering
    cv::Mat edges;
    cv::Canny(intImage, edges, thresh1, thresh2, aperture);

    cv::Mat result;
    edges.convertTo(result, CV_32F);

    // Store the result
    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::CannyEdgeDetectionOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Canny Edge Detector", "cannyEdgeDetect", category);
        return builder.
                withOperator(std::make_unique<nitro::CannyEdgeDetectionOperator>())->
                withIcon("blur.png")->
                withNodeColor({71, 47, 189})->
                withInputImage(INPUT_IMAGE)->
                withInputValue(INPUT_THRESH_1, 75, 1, 255)->
                withInputValue(INPUT_THRESH_2, 75, 1, 255)->
                withInputInteger(INPUT_APERTURE, 3, 3, 7)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
