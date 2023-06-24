#pragma once

#include "QtNodes/NodeData"
#include "flexibledata.hpp"
#include <opencv2/imgproc.hpp>
#include <QImage>
#include <utility>

namespace nitro {

    class ColImageData : public FlexibleData<std::shared_ptr<cv::Mat>, ColImageData> {
    public:
        ColImageData();


        explicit ColImageData(std::shared_ptr<cv::Mat> img);

        explicit ColImageData(const cv::Mat &img);

        static QString id() {
            return id_;
        }

        static void registerConversions();

        [[nodiscard]] QString getDescription() const override;

        [[nodiscard]] bool empty() const override;

    private:
        inline static const QString name_ = "Image";
        inline static const QString id_ = "ColImage";
        inline static const QColor baseColor_ = {199, 199, 41};
    };
} // nitro
