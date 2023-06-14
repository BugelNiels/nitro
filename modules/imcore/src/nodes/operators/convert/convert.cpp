#include "convert.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>
#include <utility>

#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

nitro::ConvertOperator::ConvertOperator(std::vector<cv::ColorConversionCodes> codes) : codes_(std::move(codes)) {

}


void nitro::ConvertOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    cv::ColorConversionCodes codec = codes_[options.at(MODE_DROPDOWN)];
    if (!nodePorts.inputsPresent({INPUT_IMAGE})) {
        return;
    }
    auto inputImg = nodePorts.getInputImage(INPUT_IMAGE);
    cv::Mat in;
    if (inputImg->channels() == 1) {
        cv::cvtColor(*inputImg, in, cv::COLOR_GRAY2RGB);
    } else {
        inputImg->copyTo(in);
    }

    cv::Mat result;
    switch (codec) {
        case cv::COLOR_Lab2RGB: {
            std::vector<cv::Mat> channels;
            cv::split(in, channels);
            channels[0] = channels[0] * 100.0;
            channels[1] = channels[1] * 255.0 - 128;
            channels[2] = channels[2] * 255.0 - 128;
            cv::merge(channels, in);
            break;
        }
        case cv::COLOR_Luv2RGB: {
            std::vector<cv::Mat> channels;
            cv::split(in, channels);
            channels[0] = channels[0] * 100.0;
            channels[1] = channels[1] * 354.0 - 134.0;
            channels[2] = channels[2] * 262.0 - 140.0;
            cv::merge(channels, in);
            break;
        }
        default:
            // These do not require any normalization
            break;
    }
    cvtColor(in, result, codec);
    // Hard coded, but necessary unfortunately
    switch (codec) {
        case cv::COLOR_RGB2Lab: {

            cv::Mat temp;
            result.convertTo(temp, CV_8U);
            temp.convertTo(result, CV_32F);
            std::vector<cv::Mat> channels;
            cv::split(result, channels);
            channels[0] = channels[0] / 100.0;
            channels[1] = channels[1] / 255 + 128 / 255.0;
            channels[2] = channels[2] / 255 + 128 / 255.0;
            cv::merge(channels, result);
            break;
        }
        case cv::COLOR_RGB2Luv: {
            std::vector<cv::Mat> channels;
            cv::split(result, channels);
            channels[0] = channels[0] / 100.0;
            channels[1] = (channels[1] + 134.0) / 354.0;
            channels[2] = (channels[2] + 140.0) / 262.0;
            cv::merge(channels, result);
            break;
        }
        default:
            // These do not require any normalization
            break;
    }


    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}


void nitro::ConvertOperator::getConversions(QStringList &colorNames, std::vector<cv::ColorConversionCodes> &codes) {
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

    colorNames.append("RGB -> CIELAB");
    codes.push_back(cv::COLOR_RGB2Lab);

    colorNames.append("CIELAB -> RGB");
    codes.push_back(cv::COLOR_Lab2RGB);

    colorNames.append("RGB -> CIELUV");
    codes.push_back(cv::COLOR_RGB2Luv);

    colorNames.append("CIELUV -> RGB");
    codes.push_back(cv::COLOR_Luv2RGB);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ConvertOperator::creator(const QString &category) {
    return [category]() {
        QStringList colorNames;
        std::vector<cv::ColorConversionCodes> codes;
        getConversions(colorNames, codes);

        nitro::NitroNodeBuilder builder("Color Space Convert", "convert", category);
        return builder.
                withOperator(std::make_unique<nitro::ConvertOperator>(codes))->
                withIcon("convert.png")->
                withNodeColor({110, 110, 29})->
                withDropDown(MODE_DROPDOWN, colorNames)->
                withInputImage(INPUT_IMAGE)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
