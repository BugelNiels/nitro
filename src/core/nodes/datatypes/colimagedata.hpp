#pragma once

#include <utility>


#include "src/core/cbdimage.hpp"
#include "QtNodes/DataInfo.hpp"
#include "QtNodes/NodeData"

namespace nitro {

    class ColorImageData : public QtNodes::NodeData {
    public:
        ColorImageData() = default;


        static QtNodes::DataInfo dataInfo() {
            return {"Image", "colImage", {199, 199, 41}};
        }

        explicit ColorImageData(std::shared_ptr<QImage> img) {
            _colImage = std::move(img);
        }

        [[nodiscard]] bool isValid() const {
            return _colImage != nullptr;
        }

        [[nodiscard]] const QImage &getDisplayImg() const {
            return *_colImage;
        }

        [[nodiscard]] QtNodes::NodeDataType type() const override {
            return QtNodes::NodeDataType{dataInfo().getDataId(), dataInfo().getDataName()};
        }

        [[nodiscard]] std::shared_ptr<QImage> image() const { return _colImage; }

    private:
        std::shared_ptr<QImage> _colImage;
    };
} // nitro