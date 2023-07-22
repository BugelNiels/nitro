#pragma once

#include <QWidget>
#include <QChartView>
#include <QValueAxis>
#include <QLineSeries>
#include "gui/imviewer/imageviewer.hpp"

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
