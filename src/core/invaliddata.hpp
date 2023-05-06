#pragma once

#include <utility>

#include "3rdparty/nodeeditor/include/QtNodes/NodeData"


#include "src/improc/core/cbdimage.hpp"

namespace nitro {
    class InvalidData : public QtNodes::NodeData {
    public:
        InvalidData() = default;

        QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{"invalid", "Invalid"}; }

    };
} // nitro