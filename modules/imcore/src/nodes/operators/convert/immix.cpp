#include "immix.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>
#include <QDebug>

#define INPUT_IMAGE_1 "Image 1"
#define INPUT_IMAGE_2 "Image 2"
#define INPUT_FAC "Fac"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

static cv::Mat blendImages(const cv::Mat &image1, const cv::Mat &image2, double alpha, double beta, double gamma) {
    cv::Mat blendedImage;
    cv::addWeighted(image1, alpha, image2, beta, gamma, blendedImage);
    return blendedImage;
}

static cv::Mat addBlend(const cv::Mat &image1, const cv::Mat &image2) {
    cv::Mat blendedImage;
    cv::add(image1, image2, blendedImage);
    return blendedImage;
}

static cv::Mat subtractBlend(const cv::Mat &image1, const cv::Mat &image2) {
    cv::Mat blendedImage;
    cv::subtract(image1, image2, blendedImage);
    return blendedImage;
}

static cv::Mat multiplyBlend(const cv::Mat &image1, const cv::Mat &image2) {
    cv::Mat blendedImage;
    cv::multiply(image1, image2, blendedImage);
    return blendedImage;
}

static cv::Mat cropToMatchSize(const cv::Mat &srcImage, const cv::Mat &targetImage) {
    int targetWidth = targetImage.cols;
    int targetHeight = targetImage.rows;

    // Create a rectangle for the ROI with the size of the target image
    cv::Rect roi(0, 0, targetWidth, targetHeight);

    // Adjust the ROI if the source image is larger than the target image
    if (srcImage.cols > targetWidth || srcImage.rows > targetHeight) {
        int offsetX = (srcImage.cols - targetWidth) / 2;
        int offsetY = (srcImage.rows - targetHeight) / 2;
        roi = cv::Rect(offsetX, offsetY, targetWidth, targetHeight);
    }

    // Crop the source image using the defined ROI
    cv::Mat croppedImage = srcImage(roi).clone();

    return croppedImage;
}

void nitro::MixOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (!nodePorts.inputsPresent({INPUT_IMAGE_1, INPUT_IMAGE_2, INPUT_FAC})) {
        return;
    }
    auto im1 = nodePorts.getInputImage(INPUT_IMAGE_1);
    auto im2 = nodePorts.getInputImage(INPUT_IMAGE_2);
    double fac = nodePorts.getInputValue(INPUT_FAC);

    int option = options.at(MODE_DROPDOWN);

    cv::Mat in1, in2;
    if (im1->size > im2->size) {
        in2 = *im2;
        in1 = cropToMatchSize(*im1, *im2);
    } else {
        in1 = *im1;
        in2 = cropToMatchSize(*im2, *im1);
    }

    if (im1->channels() == 1 && im2->channels() == 3) {
        cv::cvtColor(in1, in1, cv::COLOR_GRAY2BGR);
    }
    if (im1->channels() == 3 && im2->channels() == 1) {
        cv::cvtColor(in2, in2, cv::COLOR_GRAY2BGR);
    }

    cv::Mat result;
    switch (option) {
        case 0:
            result = blendImages(in1, in2, fac, 1 - fac, 0);
            break;
        case 1:
            result = blendImages(in1, addBlend(in1, in2), 1 - fac, fac, 0);
            break;
        case 2:
            result = blendImages(in1, subtractBlend(in1, in2), 1 - fac, fac, 0);
            break;
        case 3:
            result = blendImages(in1, multiplyBlend(in1, in2), 1 - fac, fac, 0);
            break;
        case 4:
            result = blendImages(in1, cv::min(in1, in2), 1 - fac, fac, 0);
            break;
        case 5:
            result = blendImages(in1, cv::max(in1, in2), 1 - fac, fac, 0);
            break;
        default:
            result = blendImages(in1, in2, fac, 1 - fac, 0);
            break;
    }
    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::MixOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Mix", "mix", category);
        return builder.
                withOperator(std::make_unique<nitro::MixOperator>())->
                withIcon("mix.png")->
                withNodeColor({110, 110, 29})->
                withDropDown(MODE_DROPDOWN, {"Mix", "Add", "Subtract", "Multiply", "Min", "Max"})->
                withInputValue(INPUT_FAC, 0.5, 0, 1)->
                withInputImage(INPUT_IMAGE_1)->
                withInputImage(INPUT_IMAGE_2)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
