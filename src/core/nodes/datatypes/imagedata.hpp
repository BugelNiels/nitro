#pragma once

#include <utility>

#include "QtNodes/NodeData"


#include "src/core/cbdimage.hpp"
#include "QtNodes/DataInfo.hpp"

namespace nitro {
    enum Mode {
        COLOR, GREYSCALE
    };

    class ImageData : public QtNodes::NodeData {
    public:
        ImageData() = default;


        static QtNodes::DataInfo dataInfo() {
            return {"Image", "universalImage", {199, 199, 41}};
        }

        explicit ImageData(const std::shared_ptr<CbdImage> &img) : greyImage_(img) {
            mode_ = GREYSCALE;
        }

        explicit ImageData(std::shared_ptr<QImage> img) {
            colImage_ = std::move(img);
            mode_ = COLOR;
        }

        [[nodiscard]] bool isValid() const {
            return isColImg() ? colImage_ != nullptr : greyImage_ != nullptr;
        }

        [[nodiscard]] const QImage &getDisplayImg() const {
            return isColImg() ? *colImage_ : greyImage_->getDisplayImg();
        }

        [[nodiscard]] bool isGrayscaleImg() const {
            return mode_ == GREYSCALE;
        }

        [[nodiscard]] bool isColImg() const {
            return mode_ == COLOR;
        }

        [[nodiscard]] QtNodes::NodeDataType type() const override {
            return QtNodes::NodeDataType{dataInfo().getDataId(), dataInfo().getDataName()};
        }

        [[nodiscard]] std::shared_ptr<CbdImage> image() const { return greyImage_; }

        [[nodiscard]] std::shared_ptr<QImage> colImage() const { return colImage_; }

    private:
        Mode mode_;
        std::shared_ptr<CbdImage> greyImage_;
        std::shared_ptr<QImage> colImage_;
    };
} // nitro