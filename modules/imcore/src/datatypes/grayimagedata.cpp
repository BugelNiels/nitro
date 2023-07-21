#include "include/grayimagedata.hpp"
#include "include/colimagedata.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include "nodes/datatypes/integerdata.hpp"

namespace nitro {

GrayImageData::GrayImageData()
        : FlexibleData<std::shared_ptr<cv::Mat>, GrayImageData>(std::make_shared<cv::Mat>(),
                                                                id_,
                                                                name_,
                                                                baseColor_) {
    allowConversionFrom(ColImageData::id());
}


GrayImageData::GrayImageData(std::shared_ptr<cv::Mat> img)
        : FlexibleData<std::shared_ptr<cv::Mat>, GrayImageData>(std::move(img),
                                                                id_,
                                                                name_,
                                                                baseColor_) {
    allowConversionFrom(ColImageData::id());
}

GrayImageData::GrayImageData(const cv::Mat &img)
        : FlexibleData<std::shared_ptr<cv::Mat>, GrayImageData>(std::make_shared<cv::Mat>(img),
                                                                id_,
                                                                name_,
                                                                baseColor_) {
    allowConversionFrom(ColImageData::id());
}

QString GrayImageData::getDescription() const {
    return "";
}

bool GrayImageData::empty() const { return data() == nullptr || data()->empty(); }

void GrayImageData::registerConversions() {

    // Every type needs a "conversion" to itself
    GrayImageData::registerConversionFrom<GrayImageData>(
            [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                return std::static_pointer_cast<GrayImageData>(nodeData)->data();
            });

    GrayImageData::registerConversionFrom<DecimalData>(
            [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                auto imData = std::static_pointer_cast<DecimalData>(nodeData);
                double val = imData->data();
                return std::make_shared<cv::Mat>(1, 1, CV_32F, cv::Scalar(val));
            });

    GrayImageData::registerConversionFrom<IntegerData>(
            [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                auto imData = std::static_pointer_cast<IntegerData>(nodeData);
                int val = imData->data();
                return std::make_shared<cv::Mat>(1, 1, CV_32F,
                                                 cv::Scalar(val / 255.0f));
            });

    GrayImageData::registerConversionFrom<ColImageData>(
            [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                auto imData = std::static_pointer_cast<ColImageData>(nodeData);
                if (imData->data()->channels() == 1) {
                    return std::make_shared<cv::Mat>(*imData->data());
                }
                cv::Mat res;
                cv::cvtColor(*imData->data(), res, cv::COLOR_RGB2GRAY);
                res.convertTo(res, CV_32FC1);
                return std::make_shared<cv::Mat>(res);
            });
}

} // namespace nitro
