#pragma once

#include <QString>
#include <QColor>
#include <utility>

namespace nitro {

    class DataInfo {

    public:
        DataInfo(QString name, QString id, const QColor &color) : dataName_(std::move(name)),
                                                                  dataId_(std::move(id)),
                                                                  dataColor_(color) {

        }

        [[nodiscard]] const QString &getDataName() const {
            return dataName_;
        }

        [[nodiscard]] const QString &getDataId() const {
            return dataId_;
        }

        [[nodiscard]] const QColor &getDataColor() const {
            return dataColor_;
        }

    private:
        QString dataName_;
        QString dataId_;
        QColor dataColor_;
    };

} // nitro
