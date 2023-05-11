#pragma once

#include <utility>

#include "3rdparty/nodeeditor/include/QtNodes/NodeData"


#include "src/improc/core/cbdimage.hpp"
#include "nodes/datainfo.hpp"

namespace nitro {
    enum Mode {
        COLOR, GREYSCALE
    };

    class ImageData : public QtNodes::NodeData {
    public:
        ImageData() = default;


        static DataInfo dataInfo() {
            return {"Image", "cbd_image", {199, 199, 41}};
        }

        explicit ImageData(const std::shared_ptr<CbdImage>& img) : _greyImage(img) {
            mode = GREYSCALE;
        }

        explicit ImageData(std::shared_ptr<QImage> img) {
            _colImage = std::move(img);
            mode = COLOR;
        }

        bool isValid() const {
            return isColImg() ? _colImage != nullptr : _greyImage != nullptr;
        }

        const QImage &getDisplayImg() const {
            return isColImg() ? *_colImage : _greyImage->getDisplayImg();
        }

        bool isGrayscaleImg() const {
            return mode == GREYSCALE;
        }

        bool isColImg() const {
            return mode == COLOR;
        }

        QtNodes::NodeDataType type() const override {
            return QtNodes::NodeDataType{dataInfo().getDataId(), dataInfo().getDataName()};
        }

        std::shared_ptr<CbdImage> image() const { return _greyImage; }

        std::shared_ptr<QImage> colImage() const { return _colImage; }

    private:
        Mode mode;
        std::shared_ptr<CbdImage> _greyImage;
        std::shared_ptr<QImage> _colImage;
    };
} // nitro