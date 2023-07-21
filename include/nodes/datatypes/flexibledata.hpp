#pragma once

#include "QtNodes/NodeData"
#include <opencv2/imgproc.hpp>
#include <QImage>
#include <QDebug>

namespace nitro {

/**
 *
 * @tparam T Type of the data it contains
 * @tparam U Type of the derived class. Used to ensure each derived class gets its own conversion map, regardless of the type of T.
 */
template<class T, class U>
class FlexibleData : public QtNodes::NodeData {
public:
    using DataType = T;

    FlexibleData() = default;

    explicit FlexibleData(T val, const QString &id, const QString &name, QColor color)
            : data_(val),
              type_({id, name, color}) {

    }

    [[nodiscard]] QString getDescription() const override {
        return "";
    }

    [[nodiscard]] T data() const {
        return data_;
    }

    [[nodiscard]] static T from(const std::shared_ptr<QtNodes::NodeData> &data) {
        if (data == nullptr) {
            throw std::invalid_argument("Data is null.\n");
        }

        QString id = data->type().id;
        if (conversions.count(id) == 0) {
            throw std::invalid_argument(
                    QString("Conversion from %1 is not supported. Id %2 not found.\n").arg(data->type().name,
                                                                                           id).toStdString());
        }
        return conversions[id](data);
    }


    [[nodiscard]] bool empty() const override { return false; }

    void allowConversionFrom(const QString &id) final {
        type_.allowedFromConversions.insert(id);
    }

    [[nodiscard]] QtNodes::NodeDataType type() const override {
        return type_;
    }

protected:
    template<class V>
    static void registerConversionFrom(std::function<T(const std::shared_ptr<QtNodes::NodeData> &)> conversion) {
        QString id = V::id();
        if (conversions.count(id) > 0) {
        }
        conversions[id] = std::move(conversion);
    }

private:
    // TODO: T matches for certain subclasses
    inline static std::unordered_map<QString, std::function<T(
            const std::shared_ptr<QtNodes::NodeData> &)>> conversions;

    T data_;
    QtNodes::NodeDataType type_;
};

} // namespace nitro
