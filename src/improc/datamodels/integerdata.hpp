#pragma once

#include <utility>

#include "3rdparty/nodeeditor/include/QtNodes/NodeData"


#include "src/improc/core/cbdimage.hpp"

namespace nitro {
    class IntegerData : public QtNodes::NodeData {
    public:
        IntegerData() = default;

        // TODO: check efficiency here?
        explicit IntegerData(int val) {
            val_ = val;
        }

        QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{"decimal", "Value"}; }

        int value() const { return val_; }

    private:
        int val_;
    };
} // nitro