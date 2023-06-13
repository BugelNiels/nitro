#include "gui/zoombar.hpp"

nitro::ZoomBar::ZoomBar(int minZoom, int maxZoom, QWidget *parent)
        : QProgressBar(parent),
          minZoom_(minZoom),
          maxZoom_(maxZoom) {
    setValue(50);
    curZoom_ = 100;
}

QString nitro::ZoomBar::text() const {
    return QString::number(curZoom_) + "%";
}

void nitro::ZoomBar::setZoom(int zoom) {
    curZoom_ = zoom;
    double logValue = log10(curZoom_);
    double logMin = log10(minZoom_);
    double logMax = log10(maxZoom_);
    double logPercent = (logValue - logMin) / (logMax - logMin);

    int percent = logPercent * 100;
    setValue(percent);
    update();
}

void nitro::ZoomBar::setZoom(double zoomFactor) {
    setZoom(int(zoomFactor * 100.0 + 0.5));
}
