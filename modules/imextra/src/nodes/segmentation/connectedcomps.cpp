#include "connectedcomps.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_THRESH "Threshold"
#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"
#define OPTION_INVERSE "Inverse"

void nitro::ConnectedCompsOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    // Get the input data
    auto inputImg = nodePorts.inGet<ImageData>(INPUT_IMAGE).data();
    int option = options.at(OPTION_INVERSE);
    int connectivity = option == 0 ? 4 : 8;

    double threshold = nodePorts.inputValue(INPUT_THRESH);
    cv::Mat imIn;
    if (inputImg->channels() > 1) {
        cvtColor(*inputImg, imIn, cv::COLOR_RGB2GRAY);
    } else {
        inputImg->copyTo(imIn);
    }
    cv::threshold(imIn, imIn, threshold, 1, cv::THRESH_BINARY);
    imIn.convertTo(imIn, CV_8UC1, 255);

    cv::Mat result;
    int numComps = cv::connectedComponents(imIn, result, connectivity, CV_16U);
    result.convertTo(result, CV_32F, 1.0 / numComps);

    // Store the result
    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ConnectedCompsOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Connected Components", "connectedComps", category);
        return builder.
                withOperator(std::make_unique<nitro::ConnectedCompsOperator>())->
                withIcon("connected_comps.png")->
                withNodeColor(NITRO_SEGMENTATION_COLOR)->
                withInputValue(INPUT_THRESH, 0.5, 0, 1)->
                withInputPort<ImageData>(INPUT_IMAGE)->
                withDropDown(OPTION_INVERSE, {"4-connectivity", "8-connectivity"})->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
