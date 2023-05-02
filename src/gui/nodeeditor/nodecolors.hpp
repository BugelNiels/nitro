#pragma once

#include <QString>
#include <QColor>
#include <QMap>

namespace nitro {
    class NodeColors {

    public:
        static void initColors();
        static QColor getColor(const QString& name);

    private:
        static QMap<QString, QColor> colors;

    };
} // nitro