#include "convert.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>
#include <utility>


nitro::ConvertOperator::ConvertOperator(std::vector<cv::ColorConversionCodes> codes) : codes_(std::move(codes)) {

}


void nitro::ConvertOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {


    cv::ColorConversionCodes codec = codes_[options.at("Mode")];

    bool imPresent;
    auto inputImg = nodePorts.getInputImage("Image", imPresent);
    if (!imPresent) {
        return;
    }

    cv::Mat result;
    if (inputImg->channels() == 1) {
        cv::cvtColor(*inputImg, result, cv::COLOR_GRAY2BGR);
    } else {
        inputImg->copyTo(result);
    }

    cvtColor(result, result, codec);

    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));


}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ConvertOperator::creator(const QString &category) {
    return [category]() {
        QStringList colorNames;
        std::vector<cv::ColorConversionCodes> codes;
        colorNames.append("RGB -> XYZ");
        codes.push_back(cv::COLOR_RGB2XYZ);

        colorNames.append("XYZ -> RGB");
        codes.push_back(cv::COLOR_XYZ2RGB);

        colorNames.append("RGB -> YCrCb");
        codes.push_back(cv::COLOR_RGB2YCrCb);

        colorNames.append("YCrCb -> RGB");
        codes.push_back(cv::COLOR_YCrCb2RGB);

        colorNames.append("RGB -> HSV");
        codes.push_back(cv::COLOR_RGB2HSV);

        colorNames.append("HSV -> RGB");
        codes.push_back(cv::COLOR_HSV2RGB);

        colorNames.append("RGB -> LAB");
        codes.push_back(cv::COLOR_RGB2Lab);

        colorNames.append("LAB -> RGB");
        codes.push_back(cv::COLOR_Lab2RGB);

        nitro::NitroNodeBuilder builder("Convert", "convert", category);
        return builder.
                withOperator(std::make_unique<nitro::ConvertOperator>(codes))->
                withIcon(":/icons/nodes/convert.png")->
                withNodeColor({110, 110, 29})->
                withDropDown("Mode", colorNames)->
                withInputImage("Image")->
                withOutputImage("Image")->
                build();
    };
}
