#pragma once

#include "QtNodes/NodeData"
#include "QtNodes/DataInfo.hpp"
#include <opencv2/imgproc.hpp>
#include <QImage>

namespace nitro {

    class ImageData : public QtNodes::NodeData {
    public:
        ImageData() = default;


        static QtNodes::DataInfo dataInfo() {
            return {"Image", "universalImage", {199, 199, 41}};
        }

        explicit ImageData(const std::shared_ptr<cv::Mat> &img) : image_(img) {}

        [[nodiscard]] bool isGrayscaleImg() const {
            return image_->channels() == 1;
        }

        [[nodiscard]] bool isColImg() const {
            return image_->channels() > 1;
        }

        [[nodiscard]] QtNodes::NodeDataType type() const override {
            return QtNodes::NodeDataType{dataInfo().getDataId(), dataInfo().getDataName()};
        }

        [[nodiscard]] std::shared_ptr<cv::Mat> image() const { return image_; }

    private:
        std::shared_ptr<cv::Mat> image_;
    };
} // nitro