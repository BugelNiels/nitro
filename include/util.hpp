#pragma once

#include <QImage>
#include <opencv2/opencv.hpp>

namespace nitro {

/**
 * @brief Describes the different modes for resizing.
 */
enum class AspectRatioMode {
    /**
     * @brief Resizes the image to width/height.
     * If the new aspect ratio is different from the original aspect ration, the image might appear distorted.
     */
    IGNORE,
    /**
     * @brief Resizes the image to width/height, but does not distort the image. Instead, crops the larger dimension.
     */
    KEEP_CROP,
    /**
     * @brief The image is resized to fit in the bounding box determined by the width/height.
     * In case that the aspect ratio is different, either the new width or height shrinks to maintain the aspect ratio.
     */
    KEEP_SHRINK,
    /**
     * @brief The image is resized to be at least as large as the bounding box determined by the width/height.
     * In case that the aspect ratio is different, either the new width or height grows to maintain the aspect ratio.
     */
    KEEP_GROW
};

/**
 * @brief Makes a color readable depending on whether the background is dark or light.
 *
 * @param color The color to adjust.
 * @param lightMode True if the background is light (i.e. the colour should potentially be made darker).
 * False if the background is dark (i.e. the color should potentially be made lighter).
 * @return The adjusted color.
 */
QColor makeReadable(const QColor &color, bool lightMode = false);

/**
 * @brief Converts an OpenCV image to a QImage.
 * @param img The openCV Image
 * @param buffer A temporary buffer used to store the conversion so that this does not have to be re-allocated all the time.
 * @return A QImage.
 */
QImage cvMatToQImage(const cv::Mat &src, cv::Mat &buffer);

/**
 * @brief Returns the maximum possible value that can be represented by the provided image.
 * Not to be confused with the maximum value IN the image.
 * @param img The matrix to find the maximum representable value of.
 * @return The maximum representable value.
 */
int getMaxValue(const cv::Mat &img);

/**
 * @brief Determines whether an image is grayscale or not. Also works for images consisting of multiple channels.
 * @param img The input image.
 * @return True if the image is grayscale, false otherwise.
 */
bool isGrayscale(const cv::Mat &img);

/**
 * @brief Resizes an input image to the desired target size.
 *
 * @param imIn The image to resize.
 * @param targetSize The desired size.
 * @param mode The interpolation mode. @see cv::InterpolationFlags.
 * @param arMode Aspect ratio mode. @see nitro::AspectRatioMode.
 * @return The resized image.
 */
cv::Mat resize(const cv::Mat &imIn,
               const cv::Size &targetSize,
               cv::InterpolationFlags mode,
               AspectRatioMode arMode);

} // namespace nitro
