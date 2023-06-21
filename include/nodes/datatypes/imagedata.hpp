#pragma once

#include "QtNodes/NodeData"
#include "nodes/datainfo.hpp"
#include "flexibledata.hpp"
#include <opencv2/imgproc.hpp>
#include <QImage>
#include <utility>

namespace nitro {

    class ImageData : public FlexibleData<std::shared_ptr<cv::Mat>> {
    public:
        ImageData() : FlexibleData<std::shared_ptr<cv::Mat>>(std::make_shared<cv::Mat>(), dataInfo(),
                                                             baseColor_) {}


        explicit ImageData(std::shared_ptr<cv::Mat> img)
                : FlexibleData<std::shared_ptr<cv::Mat>>(std::move(img),
                                                         dataInfo(),
                                                         baseColor_) {}

        explicit ImageData(const cv::Mat &img)
                : FlexibleData<std::shared_ptr<cv::Mat>>(std::make_shared<cv::Mat>(img),
                                                         dataInfo(),
                                                         baseColor_) {}

        static nitro::DataInfo dataInfo() {
            return {name_, id_, baseColor_};
        }

        [[nodiscard]] std::shared_ptr<cv::Mat> image() const { return data(); }

        [[nodiscard]] QString getDescription() const override {
            return "";
        }

        [[nodiscard]] bool empty() const override { return data() == nullptr || data()->empty(); }

    private:
        inline static const QString name_ = "Image";
        inline static const QString id_ = "Image";
        inline static const QColor baseColor_ = {199, 199, 41};
    };
} // nitro
