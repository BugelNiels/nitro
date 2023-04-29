#pragma once

#include <QDockWidget>
#include "src/gui/imgviewer/imgviewer.hpp"

namespace nitro {

    class NodeView : public QDockWidget {
    public:

        NodeView(nitro::ImageViewer *imViewer, QWidget *parent);

        ~NodeView() override;

    private:
    };

} // nitro