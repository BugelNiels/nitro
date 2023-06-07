#pragma once

#include <utility>


#include "src/core/cbdimage.hpp"
#include "QtNodes/DataInfo.hpp"
#include "QtNodes/NodeData"

namespace nitro {

    class GreyImageData : public QtNodes::NodeData {
    public:
        GreyImageData() = default;

        static QtNodes::DataInfo dataInfo() {
            return {"Image", "greyImage", {150, 150, 150}};
        }

        explicit GreyImageData(std::shared_ptr<CbdImage> img) {
            image_ = std::move(img);
        }

        [[nodiscard]] bool isValid() const {
            return image_ != nullptr;
        }

        [[nodiscard]] const QImage &getDisplayImg() const {
            return image_->getDisplayImg();
        }

        [[nodiscard]] QtNodes::NodeDataType type() const override {
            return QtNodes::NodeDataType{dataInfo().getDataId(), dataInfo().getDataName()};
        }

        [[nodiscard]] std::shared_ptr<CbdImage> image() const { return image_; }

    private:
        std::shared_ptr<CbdImage> image_;
    };
} // nitro