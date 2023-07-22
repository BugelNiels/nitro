#include "histogramviewer.hpp"

#include <QLineSeries>
#include <QChart>
#include <opencv2/imgproc.hpp>
#include <QApplication>
#include <QValueAxis>
#include <QAreaSeries>

namespace nitro::ImCore {

HistogramView::HistogramView(QWidget *parent) : QChartView(parent) {
    chart_ = new QChart();
    chart_->legend()->hide();
    chart_->setTheme(QChart::ChartThemeDark);
    QColor backgroundCol = QApplication::palette().color(QPalette::Window);
    chart_->setBackgroundBrush(backgroundCol);

    yAxis_ = new QValueAxis();
    chart_->addAxis(yAxis_, Qt::AlignLeft);

    xAxis_ = new QValueAxis();
    chart_->addAxis(xAxis_, Qt::AlignBottom);
    xAxis_->setGridLineVisible(true);
    yAxis_->setGridLineVisible(true);
    setChart(chart_);
    setRenderHint(QPainter::Antialiasing);
}

void HistogramView::addHistSeries(cv::Mat &singleChannelImg, const QColor& color) {
    int histSize = 256;
    float range[] = {0, 1};
    const float *histRange = {range};
    bool uniform = true;
    bool accumulate = false;
    cv::Mat histogram;
    cv::calcHist(&singleChannelImg, 1, nullptr, cv::Mat(), histogram, 1, &histSize, &histRange, uniform, accumulate);
    cv::Mat normalizedHist;
    cv::normalize(histogram, normalizedHist, 0, 1, cv::NORM_MINMAX, CV_32F);

    QLineSeries *series = new QLineSeries();
    QLineSeries *bottomSeries = new QLineSeries();
    float *histRow = normalizedHist.ptr<float>();
    for (int i = 0; i < histSize; i++) {
        float x = i * 1.0 / (histSize - 1);
        series->append(x, histRow[i]);
        bottomSeries->append(x, 0);
    }

    QAreaSeries* areaSeries = new QAreaSeries(series, bottomSeries);
    chart_->addSeries(areaSeries);
    QColor areaColor = color;
    areaColor.setAlpha(50);
    areaSeries->setBrush(QBrush(areaColor));
    areaSeries->setColor(areaColor);

    chart_->addSeries(series);
    series->setBrush(QBrush(color));
    series->attachAxis(xAxis_);
    series->attachAxis(yAxis_);
    series->setColor(color);
}

void HistogramView::updateChart(const cv::Mat &img) {
    chart_->removeAllSeries();
    if(img.empty()) {
        return;
    }

    std::vector<cv::Mat> channels;
    cv::split(img, channels);
    if(channels.size() == 1) {
        addHistSeries(channels[0], {255, 255, 255});
    } else if(channels.size() == 3) {
        // RGB
        addHistSeries(channels[0], {255, 0, 0});
        addHistSeries(channels[1], {0, 255, 0});
        addHistSeries(channels[2], {0, 0, 255});

        // Luminance plot
        cv::Mat yuvImg;
        cv::cvtColor(img, yuvImg, cv::COLOR_BGR2YUV);
        cv::split(yuvImg, channels);

        addHistSeries(channels[0], {255, 255, 255});
    }

}
} // namespace nitro::ImCore
