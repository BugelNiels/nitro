#include "zoombar.hpp"

nitro::ZoomBar::ZoomBar(int minZoom, int maxZoom, QWidget *parent) : QProgressBar(parent), m_minZoom(minZoom), m_maxZoom(maxZoom) {
    setValue(50);
    m_curZoom = 100;

}

QString nitro::ZoomBar::text() const {
    return QString::number(m_curZoom) + "%";
}

void nitro::ZoomBar::setZoom(int zoom) {
    m_curZoom = zoom;
    double logValue = log10(m_curZoom);
    double logMin = log10(m_minZoom);
    double logMax = log10(m_maxZoom);
    double logPercent = (logValue - logMin) / (logMax - logMin);

    int percent = logPercent * 100;
    setValue(percent);
    update();
}

void nitro::ZoomBar::setZoom(double zoomFactor) {
    setZoom(int(zoomFactor * 100.0 + 0.5));
}
