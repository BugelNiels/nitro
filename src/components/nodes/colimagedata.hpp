#pragma once

#include <QtNodes/NodeData>


#include "cbdimage.hpp"

namespace nitro {
    class ColImageData : public QtNodes::NodeData {
    public:
        ColImageData() = default;

        // TODO: check efficiency here?
        explicit ColImageData(const QImage &img) { _image = img; }


        QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{"col_image", "Image"}; }

        QImage image() const { return _image; }

    private:
        QImage _image;
    };
} // nitro