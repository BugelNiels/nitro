#pragma once

#include <utility>

#include "3rdparty/nodeeditor/include/QtNodes/NodeData"


#include "src/improc/core/cbdimage.hpp"

namespace nitro {
    class DecimalData : public QtNodes::NodeData {
    public:
        DecimalData() = default;

        // TODO: check efficiency here?
        explicit DecimalData(double val) {
            val_ = val;
        }

        QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType{"decimal", "Value"}; }

        double value() const { return val_; }

    private:
        double val_;
    };
} // nitro