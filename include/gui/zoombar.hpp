#pragma once

#include <QProgressBar>

namespace nitro {
    /**
     *
     */
    class ZoomBar : public QProgressBar {
    public:
        /**
         *
         * @param minZoom
         * @param maxZoom
         * @param parent
         */
        explicit ZoomBar(int minZoom, int maxZoom, QWidget *parent = nullptr);

        /**
         *
         * @return
         */
        [[nodiscard]] QString text() const override;

        /**
         *
         * @param zoom
         */
        void setZoom(int zoom);

        /**
         *
         * @param zoomFactor
         */
        void setZoom(double zoomFactor);
    private:
        int curZoom_;
        int minZoom_;
        int maxZoom_;
    };
} // nitro


