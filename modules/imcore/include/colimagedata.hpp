#pragma once

#include "nodes/datatypes/flexibledata.hpp"
#include <opencv2/imgproc.hpp>

namespace nitro {

/**
 * @brief Describes a data type for color images.
 */
class ColImageData : public FlexibleData<std::shared_ptr<cv::Mat>, ColImageData> {
public:
    /**
     * @brief Creates a new empty color image data type. Does not allocate any memory (i.e. the image is a nullptr).
     */
    ColImageData();

    /**
     * @brief Creates a color image data type with the provided image.
     * @param img The image this type should have.
     */
    explicit ColImageData(std::shared_ptr<cv::Mat> img);

    /**
     * @brief Creates a color image data type with the provided image.
     * @param img The image this type should have.
     */
    explicit ColImageData(const cv::Mat &img);

    /**
     * @brief Returns the unique id of this data type.
     * @return The unique id of this data type.
     */
    static QString id() { return id_; }

    /**
     * @brief Registers the conversions that this data type allows.
     */
    static void registerConversions();

    /**
     * @brief Retrieves the description of this data type.
     * @return The description of this data type.
     */
    [[nodiscard]] QString getDescription() const override;

    /**
     * @brief Checks whether the data is empty or not.
     * @return True if the data is empty; false otherwise.
     */
    [[nodiscard]] bool empty() const override;

private:
    inline static const QString name_ = "ColImage";
    inline static const QString id_ = "ColImage";
    inline static const QColor baseColor_ = {199, 199, 41};
};

} // namespace nitro
