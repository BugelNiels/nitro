#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include "nodes/datatypes/integerdata.hpp"

namespace nitro {
    ColImageData::ColImageData() : FlexibleData<std::shared_ptr<cv::Mat>>(std::make_shared<cv::Mat>(), id_, name_,
                                                                          baseColor_) {
        allowConversionFrom(GrayImageData::id());
    }


    ColImageData::ColImageData(std::shared_ptr<cv::Mat> img)
            : FlexibleData<std::shared_ptr<cv::Mat>>(std::move(img),
                                                     id_, name_,
                                                     baseColor_) {
        allowConversionFrom(GrayImageData::id());
    }

    ColImageData::ColImageData(const cv::Mat &img)
            : FlexibleData<std::shared_ptr<cv::Mat>>(std::make_shared<cv::Mat>(img),
                                                     id_, name_,
                                                     baseColor_) {
        allowConversionFrom(GrayImageData::id());
    }

    QString ColImageData::getDescription() const {
        return "";
    }

    bool ColImageData::empty() const { return data() == nullptr || data()->empty(); }


    void ColImageData::registerConversions() {

        // Every type needs a "conversion" to itself
        ColImageData::registerConversionFrom(ColImageData::id(),
                                             [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
            qDebug() << "From color to color";
                                                 return std::static_pointer_cast<ColImageData>(nodeData)->data();
                                             });

        ColImageData::registerConversionFrom(DecimalData::id(),
                                             [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                                                 qDebug() << "From decimal to color";
                                                 auto imData = std::static_pointer_cast<DecimalData>(nodeData);
                                                 double val = imData->data();
                                                 return std::make_shared<cv::Mat>(1, 1, CV_32F,
                                                                                  cv::Scalar(val, val, val));
                                             });

        ColImageData::registerConversionFrom(IntegerData::id(),
                                             [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                                                 qDebug() << "From integer to color";
                                                 auto imData = std::static_pointer_cast<IntegerData>(nodeData);
                                                 double val = imData->data() / 255.0;
                                                 return std::make_shared<cv::Mat>(1, 1, CV_32F,
                                                                                  cv::Scalar(val, val, val));
                                             });

        ColImageData::registerConversionFrom(GrayImageData::id(),
                                             [](const std::shared_ptr<QtNodes::NodeData> &nodeData) {
                                                 qDebug() << "From gray to color";
                                                 auto imData = std::static_pointer_cast<GrayImageData>(nodeData);
                                                 cv::Mat res;
                                                 cv::cvtColor(*imData->data(), res, cv::COLOR_GRAY2RGB);
                                                 return std::make_shared<cv::Mat>(res);
                                             });
    }

} // nitro
