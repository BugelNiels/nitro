#pragma once

#include "QtNodes/NodeData"
#include "nodes/datainfo.hpp"
#include <opencv2/imgproc.hpp>
#include <QImage>
#include <QDebug>

namespace nitro {

    template<class T>
    class FlexibleData : public QtNodes::NodeData {
    public:
        FlexibleData() = default;

        explicit FlexibleData(T val, const DataInfo &info, QColor color)
                : data_(val),
                  type_({info.getDataId(), info.getDataName(), color}) {}

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
            auto flexDat = std::dynamic_pointer_cast<FlexibleData<T>>(data);
            if (flexDat != nullptr) {
                return flexDat->data();
            }

            QString id = data->type().name;
            if (conversions.count(id) == 0) {
                throw std::invalid_argument(
                        QString("Conversion from %1 is not supported. Id %2 not found.\n").arg(data->type().name,
                                                                                               id).toStdString());
            }
            return conversions[id](data);
        }

        static void registerConversion(const DataInfo &info,
                                       std::function<T(const std::shared_ptr<QtNodes::NodeData> &)> conversion) {
            conversions[info.getDataName()] = std::move(conversion);
        }

        [[nodiscard]] bool empty() const override { return false; }

        void allowConversionFrom(const QtNodes::NodeDataType &type) override {
            type_.allowedFromConversions.insert(type.id);
        }

        [[nodiscard]] QtNodes::NodeDataType type() const override {
            return type_;
        }

    private:
        static std::map<QString, std::function<T(const std::shared_ptr<QtNodes::NodeData> &)>> conversions;

        T data_;
        QtNodes::NodeDataType type_;
    };

    template<class T>
    std::map<QString, std::function<T(const std::shared_ptr<QtNodes::NodeData> &)>> FlexibleData<T>::conversions;
} // nitro
