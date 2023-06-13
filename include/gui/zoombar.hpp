#pragma once

#include <QProgressBar>

namespace nitro {
    class ZoomBar : public QProgressBar {
    public:
        explicit ZoomBar(int minZoom, int maxZoom, QWidget *parent = nullptr);

        QString text() const override;
        void setZoom(int zoom);
        void setZoom(double zoomFactor);
    private:
        int curZoom_;
        int minZoom_;
        int maxZoom_;
    };
} // nitro


