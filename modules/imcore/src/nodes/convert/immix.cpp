#include "immix.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>
#include <QDebug>
#include "util.hpp"
#include "nodes/datatypes/imagedata.hpp"

#define INPUT_IMAGE_1 "Image 1"
#define INPUT_IMAGE_2 "Image 2"
#define INPUT_FAC "Fac"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

static cv::Mat blendImages(const cv::Mat &image1, const cv::Mat &image2, double alpha, double beta) {
    cv::Mat blendedImage;
    cv::addWeighted(image1, alpha, image2, beta, 0.0, blendedImage);
    return blendedImage;
}

static cv::Mat addBlend(const cv::Mat &image1, const cv::Mat &image2, const cv::Mat &mask) {
    cv::Mat blendedImage;
    image1.copyTo(blendedImage);
    cv::add(image1, image2, blendedImage, mask);
    return blendedImage;
}

static cv::Mat subtractBlend(const cv::Mat &image1, const cv::Mat &image2, const cv::Mat &mask) {
    cv::Mat blendedImage;
    image1.copyTo(blendedImage);
    cv::subtract(image1, image2, blendedImage, mask);
    return blendedImage;
}

static cv::Mat multiplyBlend(const cv::Mat &image1, const cv::Mat &image2) {
    cv::Mat blendedImage;
    cv::multiply(image1, image2, blendedImage);
    return blendedImage;
}

void
nitro::MixOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto im1 = nodePorts.inGet<ImageData>(INPUT_IMAGE_1).data();
    auto im2 = nodePorts.inGet<ImageData>(INPUT_IMAGE_2).data();
    double fac = nodePorts.inputValue(INPUT_FAC);

    int option = options.at(MODE_DROPDOWN);

    cv::Mat in1;
    cv::Mat in2;
    im1->copyTo(in1);
    im2->copyTo(in2);

    if (im1->channels() == 1 && im2->channels() == 3) {
        cv::cvtColor(in1, in1, cv::COLOR_GRAY2BGR);
    }
    if (im1->channels() == 3 && im2->channels() == 1) {
        cv::cvtColor(in2, in2, cv::COLOR_GRAY2BGR);
    }
    in2 = cropToMatchSize(in2, in1);


    cv::Mat result;
    switch (option) {
        case 0:
            result = blendImages(in1, in2, fac, 1 - fac);
            break;
        case 1: {
            cv::Mat mask = createMask(im2->size, in1.size);
            result = blendImages(in1, addBlend(in1, in2, mask), 1 - fac, fac);
            break;
        }
        case 2: {
            cv::Mat mask = createMask(im2->size, in1.size);
            result = blendImages(in1, subtractBlend(in1, in2, mask), 1 - fac, fac);
            break;
        }
        case 3:
            result = blendImages(in1, multiplyBlend(in1, in2), 1 - fac, fac);
            break;
        case 4:
            result = blendImages(in1, cv::min(in1, in2), 1 - fac, fac);
            break;
        case 5:
            result = blendImages(in1, cv::max(in1, in2), 1 - fac, fac);
            break;
        default:
            result = blendImages(in1, in2, fac, 1 - fac);
            break;
    }
    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
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
                withInputPort<ImageData>(INPUT_IMAGE_1)->
                withInputPort<ImageData>(INPUT_IMAGE_2)->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
