#include "ZoomBar.h"

nitro::ZoomBar::ZoomBar(int minZoom, int maxZoom, QWidget *parent) : QProgressBar(parent), m_minZoom(minZoom), m_maxZoom(maxZoom) {
    setValue(50);
    m_curZoom = 100;

}

QString nitro::ZoomBar::text() const {
    return QString::number(m_curZoom) + "%";
    QString text = QString::number(value()) + "/" + QString::number(maximum());
    if (maximum() > minimum()) {
        int percentage = (value() - minimum()) * 100 / (maximum() - minimum());
        text += " (" + QString::number(percentage) + "%)";
    }
    return text;
}

void nitro::ZoomBar::setZoom(int zoom) {
    m_curZoom = zoom;
    double logValue = log10(m_curZoom);
    double logMin = log10(m_minZoom);
    double logMax = log10(m_maxZoom);
    double logPercent = (logValue - logMin) / (logMax - logMin);

// Convert the logarithmic percentage to a range between 0 and 100
    int percent = logPercent * 100;

// Set the range and value of the progress bar
//    setRange(0, 100);
    setValue(percent);
    update();
}

void nitro::ZoomBar::setZoom(double zoomFactor) {
    setZoom(int(zoomFactor * 100.0 + 0.5));
}
