#include "nodes/datatypes/grayimagedata.hpp"
#include "nodes/datatypes/colimagedata.hpp"

namespace nitro {
    GrayImageData::GrayImageData() : FlexibleData<std::shared_ptr<cv::Mat>>(std::make_shared<cv::Mat>(), id_, name_,
                                                                            baseColor_) {
        allowConversionFrom(ColImageData::id());
    }


    GrayImageData::GrayImageData(std::shared_ptr<cv::Mat> img)
            : FlexibleData<std::shared_ptr<cv::Mat>>(std::move(img),
                                                     id_, name_,
                                                     baseColor_) {
        allowConversionFrom(ColImageData::id());
    }

    GrayImageData::GrayImageData(const cv::Mat &img)
            : FlexibleData<std::shared_ptr<cv::Mat>>(std::make_shared<cv::Mat>(img),
                                                     id_, name_,
                                                     baseColor_) {
        allowConversionFrom(ColImageData::id());
    }

    QString GrayImageData::getDescription() const {
        return "";
    }

    bool GrayImageData::empty() const { return data() == nullptr || data()->empty(); }

} // nitro
