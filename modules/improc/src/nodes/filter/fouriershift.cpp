#include "fouriershift.hpp"
#include <grayimagedata.hpp>
#include <nodes/nitronodebuilder.hpp>

#include <opencv2/imgproc.hpp>

namespace nitro::ImProc {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString OUTPUT_IMAGE = "Image";

// Credit: https://stackoverflow.com/a/57673157
bool fftShift(const cv::Mat &src, cv::Mat &dst) {
    if (src.empty())
        return true;

    const int h = src.rows;
    const int w = src.cols; // height and width of src-image
    const int qh = h / 2;
    const int qw = w / 2; // height and width of the quadrants

    cv::Mat qTL(src, cv::Rect(0, 0, qw, qh));           // define the quadrants in respect to
    cv::Mat qTR(src, cv::Rect(w - qw, 0, qw, qh));      // the outer dimensions of the matrix.
    cv::Mat qBL(src, cv::Rect(0, h - qh, qw, qh));      // thus, with odd sizes, the center
    cv::Mat qBR(src, cv::Rect(w - qw, h - qh, qw, qh)); // line(s) get(s) omitted.

    cv::Mat tmp;
    hconcat(qBR, qBL, dst); // build destination matrix with switched
    hconcat(qTR, qTL, tmp); // quadrants 0 & 2 and 1 & 3 from source
    vconcat(dst, tmp, dst);

    return false;
}

void FFTShiftOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    // Get the input data
    auto inputImg = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);

    cv::Mat result;
    fftShift(*inputImg, result);

    // Store the result
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> FFTShiftOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Fourier Shift", "fftShift", category);
        return builder.withOperator(std::make_unique<FFTShiftOperator>())
                ->withIcon("shift.png")
                ->withNodeColor(NITRO_FILTER_COLOR)
                ->withInputPort<GrayImageData>(INPUT_IMAGE)
                ->withOutputPort<GrayImageData>(OUTPUT_IMAGE)
                ->build();
    };
}

} // namespace nitro::ImProc
