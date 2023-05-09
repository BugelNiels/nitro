#include "datacolors.hpp"

QMap<QString, QColor> nitro::DataColors::colors;


void nitro::DataColors::registerColor(const DataInfo &info) {
    colors[info.getDataId()] = info.getDataColor();
}

QColor nitro::DataColors::getColor(const QString &name) {
    if (colors.contains(name)) {
        return colors[name];
    }
    return QColor{168, 105, 49};
}

QColor nitro::DataColors::getColor(const nitro::DataInfo &info) {
    return getColor(info.getDataId());
}
