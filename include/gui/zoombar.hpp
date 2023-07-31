#pragma once

#include <QProgressBar>

namespace nitro {

/**
 * A simple zoom bar widget that can be used to display zoom percentages.
 */
class ZoomBar : public QProgressBar {
public:
    /**
     * Creates a new zoom bar.
     *
     * @param minZoom The minimum zoom percentage.
     * @param maxZoom The maximum zoom percentage.
     * @param parent Parent widget.
     */
    explicit ZoomBar(int minZoom, int maxZoom, QWidget *parent = nullptr);

    /**
     * The text to be displayed in the widget itself. Displays the zoom percentage.
     * @return The text displayed in the widget.
     */
    [[nodiscard]] QString text() const override;

    /**
     * Set the zoom to the provided percentage. (e.g. zoom=1 -> 1%).
     * @param zoom Zoom percentage.
     */
    void setZoom(int zoom);

    /**
     * Set the zoom to the provided factor. (e.g. zoom=0.1 -> 10%).
     * @param zoomFactor Zoom factor.
     */
    void setZoom(double zoomFactor);

private:
    int curZoom_;
    int minZoom_;
    int maxZoom_;
};

} // namespace nitro
