#pragma once

#include "QtNodes/NodeData"
#include "nodes/datainfo.hpp"
#include "flexibledata.hpp"
#include <opencv2/imgproc.hpp>
#include <QImage>

namespace nitro {

    class ImageData : public FlexibleData<std::shared_ptr<cv::Mat>> {
    public:
        ImageData() : FlexibleData<std::shared_ptr<cv::Mat>>(std::make_shared<cv::Mat>()) {}

        static nitro::DataInfo dataInfo() {
            return {name_, id_, colColor_};
        }

        [[nodiscard]] QtNodes::NodeDataType type() const override {
            return QtNodes::NodeDataType{dataInfo().getDataId(), dataInfo().getDataName(), color_};
        }

        // TODO: check reference
        explicit ImageData(const cv::Mat &dat) : FlexibleData(std::make_shared<cv::Mat>(dat)) {
//            color_ = dat.channels() == 1 ? greyscaleColor_ : colColor_;
        }

        explicit ImageData(std::shared_ptr<cv::Mat> dat) : FlexibleData(std::move(dat)) {
//            color_ = data()->channels() == 1 ? greyscaleColor_ : colColor_;
        }

        [[nodiscard]] std::shared_ptr<cv::Mat> image() const { return data(); }

        [[nodiscard]] QString getDescription() const override {
            return "";
        }


        [[nodiscard]] bool empty() const override { return data() == nullptr || data()->empty(); }

    private:
        inline static const QString name_ = "Image";
        inline static const QString id_ = "Image";
        inline static const QColor greyscaleColor_ = {161, 161, 161};
        inline static const QColor colColor_ = {199, 199, 41};

        QColor color_ = colColor_;

    };
} // nitro
