#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/grayimagedata.hpp"

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

} // nitro
