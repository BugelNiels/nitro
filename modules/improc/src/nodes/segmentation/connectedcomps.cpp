#include "connectedcomps.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "../../../../imcore/include/grayimagedata.hpp"

#include <opencv2/imgproc.hpp>

namespace nitro::ImProc {

inline const QString INPUT_THRESH = "Threshold";
inline const QString INPUT_IMAGE = "Image";
inline const QString OUTPUT_IMAGE = "Image";
inline const QString OPTION_INVERSE = "Inverse";

void ConnectedCompsOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    // Get the input data
    auto inputImg = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    int option = nodePorts.getOption(OPTION_INVERSE);
    int connectivity = option == 0 ? 4 : 8;

    double threshold = nodePorts.inputValue(INPUT_THRESH);
    cv::Mat imIn;
    cv::threshold(*inputImg, imIn, threshold, 1, cv::THRESH_BINARY);
    imIn.convertTo(imIn, CV_8UC1, 255);

    cv::Mat result;
    int numComps = cv::connectedComponents(imIn, result, connectivity, CV_16U);
    result.convertTo(result, CV_32F, 1.0 / numComps);

    // Store the result
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> ConnectedCompsOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Connected Components", "connectedComps", category);
        return builder.
                withOperator(std::make_unique<ConnectedCompsOperator>())->
                withIcon("connected_comps.png")->
                withNodeColor(NITRO_SEGMENTATION_COLOR)->
                withInputValue(INPUT_THRESH, 0.5, 0, 1)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withDropDown(OPTION_INVERSE, {"4-connectivity", "8-connectivity"})->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}

} // namespace nitro::ImProc
