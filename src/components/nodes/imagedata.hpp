#pragma once

#include <QtNodes/NodeData>


#include "cbdimage.hpp"

namespace nitro {
    enum Mode {
        COLOR, GREYSCALE
    };

    class ImageData : public QtNodes::NodeData {
    public:
        ImageData() = default;

        // TODO: check efficiency here?
        explicit ImageData(const CbdImage &img) {
            _image = CbdImage(img);
            mode = GREYSCALE;
        }

        explicit ImageData(const QImage &img) {
            _colImage = img;
            mode = COLOR;
        }

        bool isGrayscaleImg() const {
            return mode == GREYSCALE;
        }

        bool isColImg() const {
            return mode == COLOR;
        }

        QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{"cbd_image", "Image"}; }

        CbdImage &image() { return _image; }

        QImage &colImage() { return _colImage; }

    private:
        Mode mode;
        CbdImage _image;
        QImage _colImage;
    };
} // nitro