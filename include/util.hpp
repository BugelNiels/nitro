#pragma once

#include <QImage>
#include <opencv2/opencv.hpp>

namespace nitro {

    /**
     *
     * @param color
     * @param lightMode
     * @return
     */
    QColor makeReadable(const QColor &color, bool lightMode = false);

    /**
     *
     * @param img
     * @return
     */
    QImage cvMatToQImage(const cv::Mat &src, cv::Mat &buffer);

    /**
     *
     * @param inImage
     * @param inCloneImageData
     * @return
     */
    cv::Mat QImageToCvMat(const QImage &inImage, bool inCloneImageData);

    /**
     *
     * @param mat
     * @return
     */
    int getMaxValue(const cv::Mat &mat);

    /**
     *
     * @param img
     * @return
     */
    bool isGrayscale(const cv::Mat &img);

    /**
     *
     * @param srcImage
     * @param targetImage
     * @return
     */
    cv::Mat cropToMatchSize(const cv::Mat &srcImage, const cv::Mat &targetImage);

    /**
     *
     * @param srcSize
     * @param targetSize
     * @return
     */
    cv::Mat createMask(const cv::MatSize &srcSize, const cv::MatSize &targetSize);

    /**
     *
     * @param imIn
     * @param targetSize
     * @param mode
     * @param maintainAspectRatio
     * @return
     */
    cv::Mat resize(const cv::Mat &imIn,
                   const cv::Size &targetSize,
                   const cv::InterpolationFlags mode, bool maintainAspectRatio);
} // nitro
