#pragma once

#include <utility>

#include "3rdparty/nodeeditor/include/QtNodes/NodeData"


#include "src/improc/core/cbdimage.hpp"

namespace nitro {
    enum Mode {
        COLOR, GREYSCALE
    };

    class ImageData : public QtNodes::NodeData {
    public:
        ImageData() = default;

        // TODO: check efficiency here?
        explicit ImageData(std::shared_ptr<CbdImage> img) {
            _greyImage = std::move(img);
            mode = GREYSCALE;
        }

        explicit ImageData(std::shared_ptr<QImage> img) {
            _colImage = std::move(img);
            mode = COLOR;
        }

        bool isValid () {
            return isColImg() ?  _colImage != nullptr : _greyImage != nullptr;
        }

        const QImage &getDisplayImg() {
            // TODO: check the ref
            return isColImg() ? *_colImage :  _greyImage->getDisplayImg() ;
        }

        bool isGrayscaleImg() const {
            return mode == GREYSCALE;
        }

        bool isColImg() const {
            return mode == COLOR;
        }

        QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{"cbd_image", "Image"}; }

        std::shared_ptr<CbdImage> image() { return _greyImage; }

        std::shared_ptr<QImage> colImage() { return _colImage; }

    private:
        Mode mode;
        std::shared_ptr<CbdImage> _greyImage;
        std::shared_ptr<QImage> _colImage;
    };
} // nitro