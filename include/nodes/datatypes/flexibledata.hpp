#pragma once

#include <QDebug>
#include <QImage>
#include <QtNodes/NodeData>
#include <opencv2/imgproc.hpp>

namespace nitro {

/**
 * @brief A flexible data type that can contain any type.
 * @tparam T Type of the data it contains. This can be for example, int, double, cv::Mat or std::shared_ptr<cv::Mat>.
 * @tparam U Type of the derived class. Used to ensure each derived class gets its own conversion map, regardless of the type of T.
 * For example, if two data types both encapsulate an int, but do different, this type U is needed to differentiate between the two.
 */
template<class T, class U>
class FlexibleData : public QtNodes::NodeData {
public:
    /**
     * Used for nitro::NodePorts#inGetAs
     */
    using DataType = T;

    /**
     * @brief Creates a new empty flexible data type.
     */
    FlexibleData() = default;

    /**
     * @brief Creates a new instance of this flexible data type.
     * @param val The data this class should encapsulate.
     * @param id The id of this type. Should be unique among all types.
     * @param name The name of this data type.
     * @param color The color associated with this data type.
     */
    explicit FlexibleData(T val, const QString &id, const QString &name, QColor color)
        : data_(val),
          type_({id, name, color}) {}

    /**
     * @brief Retrieves the description of this data type.
     * @return The description of the data type. Will be displayed when the user hovers over an output port of this type.
     */
    [[nodiscard]] QString getDescription() const override { return ""; }

    /**
     * @brief Retrieves the data encapsulated by this class.
     * @return The data encapsulated by this class.
     */
    [[nodiscard]] T data() const { return data_; }

    /**
     * @brief Retrieve data of type T from the provided node data.
     * Only possible if a conversion is registered between the type encapsulated by the parameter data and this type.
     * @param data The data to convert to type T.
     * @return The converted data.
     */
    [[nodiscard]] static T from(const std::shared_ptr<QtNodes::NodeData> &data) {
        if (data == nullptr) {
            throw std::invalid_argument("Data is null.\n");
        }

        QString id = data->type().id;
        if (conversions.count(id) == 0) {
            throw std::invalid_argument(
                    QString("Conversion from %1 is not supported. Id %2 not found.\n")
                            .arg(data->type().name, id)
                            .toStdString());
        }
        return conversions[id](data);
    }


    /**
     * @brief Allow the type with the provided id to be converted to this data type.
     * @param id The id of the data type to allow the conversion from.
     */
    void allowConversionFrom(const QString &id) final { type_.allowedFromConversions.insert(id); }

    /**
     * @brief Checks whether the data is empty or not.
     * @return True if the data is empty; false otherwise.
     */
    [[nodiscard]] bool empty() const override { return false; }

    /**
     * @brief Retrieves the type of this data.
     * @return The type of this data.
     */
    [[nodiscard]] QtNodes::NodeDataType type() const override { return type_; }

protected:
    /**
     * @brief Registers a function that can convert from type A to B, where B is the data type described by the class that this method is called on.
     * Only one conversion function can exist per type of A.
     * @param V The type of the data that is being converted (i.e. type A).
     * @param conversion The conversion function that specifies how to convert from A to B.
     */
    template<class V>
    static void registerConversionFrom(
            std::function<T(const std::shared_ptr<QtNodes::NodeData> &)> conversion) {
        QString id = V::id();
        conversions[id] = std::move(conversion);
    }

private:
    inline static std::unordered_map<QString,
                                     std::function<T(const std::shared_ptr<QtNodes::NodeData> &)>>
            conversions;

    T data_;
    QtNodes::NodeDataType type_;
};

} // namespace nitro
