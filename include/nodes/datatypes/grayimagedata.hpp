#pragma once

#include "QtNodes/NodeData"
#include "flexibledata.hpp"
#include <opencv2/imgproc.hpp>
#include <QImage>
#include <utility>

namespace nitro {

    class GrayImageData : public FlexibleData<std::shared_ptr<cv::Mat>, GrayImageData> {
    public:
        GrayImageData();

        explicit GrayImageData(std::shared_ptr<cv::Mat> img);

        explicit GrayImageData(const cv::Mat &img);

        static QString id() {
            return id_;
        }

        static void registerConversions();

        [[nodiscard]] QString getDescription() const override;

        [[nodiscard]] bool empty() const override;

    private:
        inline static const QString name_ = "GrayImage";
        inline static const QString id_ = "GrayImage";
        inline static const QColor baseColor_ = {161, 161, 161};
    };
} // nitro
