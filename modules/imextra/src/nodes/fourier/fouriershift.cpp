#include "fouriershift.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"

// Credit: https://stackoverflow.com/a/57673157
bool fftShift(const cv::Mat &src, cv::Mat &dst) {
    if (src.empty()) return true;

    const int h = src.rows;
    const int w = src.cols;        // height and width of src-image
    const int qh = h / 2;
    const int qw = w / 2;              // height and width of the quadrants

    cv::Mat qTL(src, cv::Rect(0, 0, qw, qh));   // define the quadrants in respect to
    cv::Mat qTR(src, cv::Rect(w - qw, 0, qw, qh));   // the outer dimensions of the matrix.
    cv::Mat qBL(src, cv::Rect(0, h - qh, qw, qh));   // thus, with odd sizes, the center
    cv::Mat qBR(src, cv::Rect(w - qw, h - qh, qw, qh));   // line(s) get(s) omitted.

    cv::Mat tmp;
    hconcat(qBR, qBL, dst);                   // build destination matrix with switched
    hconcat(qTR, qTL, tmp);                   // quadrants 0 & 2 and 1 & 3 from source
    vconcat(dst, tmp, dst);

    return false;
}

void nitro::FFTShift::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    // Get the input data
    auto inputImg = nodePorts.inGet<ImageData>(INPUT_IMAGE).data();

    cv::Mat result;
    fftShift(*inputImg, result);

    // Store the result
    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::FFTShift::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("FFT Shift", "fftShift", category);
        return builder.
                withOperator(std::make_unique<nitro::FFTShift>())->
                withIcon("shift.png")->
                withNodeColor({47, 105, 204})->
                withInputPort<ImageData>(INPUT_IMAGE)->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
