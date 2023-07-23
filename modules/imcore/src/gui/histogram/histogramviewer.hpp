#pragma once

#include "gui/imviewer/imageviewer.hpp"
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QWidget>

namespace nitro::ImCore {

class HistogramView : public QChartView {

public:
    explicit HistogramView(QWidget *parent = nullptr);

    void updateChart(const cv::Mat &img);

private:
    QChart *chart_;

    QValueAxis *xAxis_;
    QValueAxis *yAxis_;

    void addHistSeries(cv::Mat &singleChannelImg, const QColor &color);
};

} // namespace nitro::ImCore
