#include "nodecolors.hpp"

QMap<QString, QColor> nitro::NodeColors::colors;


void nitro::NodeColors::registerColor(const NodeInfo &info) {
    colors[info.getNodeId()] = info.getNodeColor();
}

QColor nitro::NodeColors::getColor(const QString &name) {
    if (colors.contains(name)) {
        return colors[name];
    }
    return QColor{168, 105, 49};
}

QColor nitro::NodeColors::getColor(const nitro::NodeInfo &info) {
    return getColor(info.getNodeId());
}
