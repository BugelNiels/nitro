#pragma once

#include <QGraphicsScene>

namespace nitro {
    class ImageViewerScene : public QGraphicsScene {
    public:
        ImageViewerScene() = default;

        explicit ImageViewerScene(QObject *parent);

        ~ImageViewerScene() override = default;
    };

} // nitro