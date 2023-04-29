#pragma once

#include <QDockWidget>
#include "imageview.hpp"

namespace nitro {

    class NodeView : public QDockWidget {
    public:

        NodeView(nitro::ImageView *imViewer, QWidget *parent);

        ~NodeView() override;

    private:
    };

} // nitro