#include "mse.hpp"
#include <grayimagedata.hpp>
#include <nodes/datatypes/decimaldata.hpp>
#include <nodes/nitronodebuilder.hpp>
#include <util.hpp>

#include <opencv2/imgproc.hpp>

namespace nitro::ImProc {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString INPUT_IMAGE_REF = "Reference";
static inline const QString OUTPUT_VALUE = "Error";

static double mse(const cv::Mat &im1, const cv::Mat &im2) {
    cv::Mat diff;
    if (im1.size != im2.size) {
        return -1;
    }
    cv::absdiff(im1, im2, diff);
    cv::Mat squaredDiff = diff.mul(diff);
    return cv::mean(squaredDiff).val[0];
}

void MseOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto imIn = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    auto imRef = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE_REF);
    cv::Mat imIn8bit;
    cv::Mat imRef8bit;
    imIn.convertTo(imIn8bit, CV_8U, 255);
    imRef.convertTo(imRef8bit, CV_8U, 255);

    nodePorts.output<DecimalData>(OUTPUT_VALUE, mse(imIn8bit, imRef8bit));
}

std::function<std::unique_ptr<NitroNode>()> MseOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("MSE", "mse", category);
        return builder.withOperator(std::make_unique<MseOperator>())
                ->withIcon("compare.png")
                ->withNodeColor(NITRO_QUALITY_COLOR)
                ->withInputPort<GrayImageData>(INPUT_IMAGE)
                ->withInputPort<GrayImageData>(INPUT_IMAGE_REF)
                ->withOutputValue(OUTPUT_VALUE)
                ->build();
    };
}

} // namespace nitro::ImProc
