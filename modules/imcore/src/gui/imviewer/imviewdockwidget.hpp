#pragma once

#include <QDockWidget>
#include <QStatusBar>
#include <QLabel>

namespace nitro {
    class MainWindow;

    class ImageViewer;

    class ImViewDockWidget : public QDockWidget {
    public:

        explicit ImViewDockWidget(ImageViewer *imageViewer, MainWindow *window);

        ~ImViewDockWidget() override;

    private:
        ImageViewer *imageViewer_;

        QWidget *initStatusBar();

        QLabel *colLabel_;
        QLabel *xLabel_;
        QLabel *yLabel_;
        QLabel *rLabel_;
        QLabel *gLabel_;
        QLabel *bLabel_;
        QLabel *hLabel_;
        QLabel *sLabel_;
        QLabel *vLabel_;
        QLabel *lLabel_;
        QPixmap colLabelPixMap_;
        QLabel *hexLabel_;

        void updateFooterLabels(const QPoint &pos, const QColor &color);

        QWidget *initTitleBarWidget(MainWindow *window) const;
    };

} // nitro
