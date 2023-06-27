#include "psnr.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include "util.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_IMAGE_REF "Reference"
#define OUTPUT_VALUE "Error"

static double mse(const cv::Mat &im1, const cv::Mat &im2) {
    cv::Mat diff;
    cv::absdiff(im1, im2, diff);
    cv::Mat squaredDiff = diff.mul(diff);
    return cv::mean(squaredDiff).val[0];
}

void nitro::PsnrOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    auto imIn = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    auto imRef = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE_REF);
    double psnr = 10.0 * log10(1.0 / mse(imIn, imRef));
    nodePorts.output<DecimalData>(OUTPUT_VALUE, psnr);

}


std::function<std::unique_ptr<nitro::NitroNode>()> nitro::PsnrOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("PSNR", "psnr", category);
        return builder.
                withOperator(std::make_unique<nitro::PsnrOperator>())->
                withIcon("compare.png")->
                withNodeColor(NITRO_QUALITY_COLOR)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withInputPort<GrayImageData>(INPUT_IMAGE_REF)->
                withOutputValue(OUTPUT_VALUE)->
                build();
    };
}
