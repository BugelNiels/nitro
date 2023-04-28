#pragma once

#include <QtNodes/NodeData>


#include "cbdimage.hpp"

namespace nitro {
    class ImageData : public QtNodes::NodeData {
    public:
        ImageData() = default;

        // TODO: check efficiency here?
        explicit ImageData(const CbdImage &img) { _image = CbdImage(img); }


        QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{"cbd_image", "Image"}; }

        CbdImage image() const { return _image; }

    private:
        CbdImage _image;
    };
} // nitro