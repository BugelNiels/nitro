#include "colorspaceconvert.hpp"
#include <nodes/nitronodebuilder.hpp>
#include "include/colimagedata.hpp"

#include <opencv2/imgproc.hpp>
#include <utility>

namespace nitro::ImCore {

inline const QString INPUT_IMAGE = "Image";
inline const QString OUTPUT_IMAGE = "Image";
inline const QString MODE_DROPDOWN = "Mode";

ConvertOperator::ConvertOperator(std::vector<cv::ColorConversionCodes> codes) : codes_(std::move(codes)) {

}

void ConvertOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    cv::ColorConversionCodes codec = codes_[nodePorts.getOption(MODE_DROPDOWN)];
    auto inputImg = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    cv::Mat img;
    if (inputImg->channels() == 1) {
        cv::cvtColor(*inputImg, img, cv::COLOR_GRAY2RGB);
    } else {
        inputImg->copyTo(img);
    }

    switch (codec) {
        case cv::COLOR_Lab2RGB: {
            std::vector<cv::Mat> channels;
            cv::split(img, channels);
            channels[0] = channels[0] * 100.0;
            channels[1] = channels[1] * 255.0 - 128;
            channels[2] = channels[2] * 255.0 - 128;
            cv::merge(channels, img);
            break;
        }
        case cv::COLOR_Luv2RGB: {
            std::vector<cv::Mat> channels;
            cv::split(img, channels);
            channels[0] = channels[0] * 100.0;
            channels[1] = channels[1] * 354.0 - 134.0;
            channels[2] = channels[2] * 262.0 - 140.0;
            cv::merge(channels, img);
            break;
        }
        default:
            // These do not require any normalization
            break;
    }
    cvtColor(img, img, codec);
    // Hard coded, but necessary unfortunately
    switch (codec) {
        case cv::COLOR_RGB2Lab: {
            std::vector<cv::Mat> channels;
            cv::split(img, channels);
            channels[0] = channels[0] / 100.0;
            channels[1] = channels[1] / 255 + 128 / 255.0;
            channels[2] = channels[2] / 255 + 128 / 255.0;
            cv::merge(channels, img);
            break;
        }
        case cv::COLOR_RGB2Luv: {
            std::vector<cv::Mat> channels;
            cv::split(img, channels);
            channels[0] = channels[0] / 100.0;
            channels[1] = (channels[1] + 134.0) / 354.0;
            channels[2] = (channels[2] + 140.0) / 262.0;
            cv::merge(channels, img);
            break;
        }
        default:
            // These do not require any normalization
            break;
    }


    nodePorts.output<ColImageData>(OUTPUT_IMAGE, img);
}


void ConvertOperator::getConversions(QStringList &colorNames, std::vector<cv::ColorConversionCodes> &codes) {
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

std::function<std::unique_ptr<NitroNode>()> ConvertOperator::creator(const QString &category) {
    return [category]() {
        QStringList colorNames;
        std::vector<cv::ColorConversionCodes> codes;
        getConversions(colorNames, codes);

        NitroNodeBuilder builder("Convert Color Space", "convert", category);
        return builder.
                withOperator(std::make_unique<ConvertOperator>(codes))->
                withIcon("color.png")->
                withNodeColor(NITRO_COLOR_COLOR)->
                withDropDown(MODE_DROPDOWN, colorNames)->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}

} // namespace nitro::ImCore
