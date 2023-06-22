#include "imtranslate.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_X "X"
#define INPUT_Y "Y"
#define OPTION_PAD "Pad"
#define OUTPUT_IMAGE "Image"
#define INTERPOL_METHOD_LABEL "Interpolation Method"

static cv::Mat translateImagePad(const cv::Mat &image, int dx, int dy) {

    int newWidth = image.cols + std::abs(dx);
    int newHeight = image.rows + std::abs(dy);

    dx = std::max(dx, 0);
    dy = std::max(dy, 0);

    cv::Mat translationMatrix = (cv::Mat_<float>(2, 3) << 1, 0, dx, 0, 1, dy);
    cv::Mat translatedImage;
    cv::warpAffine(image, translatedImage, translationMatrix, {newWidth, newHeight});
    return translatedImage;
}

static cv::Mat translateImage(const cv::Mat &image, int dx, int dy) {
    cv::Mat translationMatrix = (cv::Mat_<float>(2, 3) << 1, 0, dx, 0, 1, dy);
    cv::Mat translatedImage;
    cv::warpAffine(image, translatedImage, translationMatrix, image.size());
    return translatedImage;
}

void nitro::TranslateOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto im1 = ColImageData::from(nodePorts.inGet(INPUT_IMAGE));
    double offsetRatioX = nodePorts.inputValue(INPUT_X);
    double offsetRatioY = nodePorts.inputValue(INPUT_Y);
    int offsetX = int(std::round(offsetRatioX * im1->cols));
    int offsetY = int(std::round(offsetRatioY * im1->rows));
    int option = options.at(OPTION_PAD);


    cv::Mat result;
    if (option) {
        result = translateImagePad(*im1, offsetX, offsetY);
    } else {
        result = translateImage(*im1, offsetX, offsetY);
    }
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::TranslateOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Translate", "translate", category);
        return builder.
                withOperator(std::make_unique<nitro::TranslateOperator>())->
                withIcon("resize.png")->
                withNodeColor(NITRO_TRANSFORM_COLOR)->
                withCheckBox(OPTION_PAD, false)->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withInputValue(INPUT_X, 0, -1, 1, BoundMode::UNCHECKED)->
                withInputValue(INPUT_Y, 0, -1, 1, BoundMode::UNCHECKED)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}
