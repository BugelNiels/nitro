#pragma once

#include <QIcon>
#include <QMap>

namespace nitro {

    class ImgResourceReader {

    public:
        static QPixmap
        getPixMap(const QString &path, const QSize &size = QSize(32, 32), const QColor &color = QColor(180, 180, 180));

    private:

        static QMap<QString, QPixmap> loadedIcons;
    };

} // nitro
