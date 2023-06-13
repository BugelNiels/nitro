#pragma once

#include <QIcon>
#include <QMap>

namespace nitro {

    class ImResourceReader {

    public:
        static QPixmap
        getPixMap(const QString &path, const QSize &size = QSize(42, 42), const QColor &color = QColor(180, 180, 180));

    private:

        static QMap<QString, QPixmap> loadedIcons_;

    };

} // nitro
