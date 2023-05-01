#pragma once

#include <QIcon>
#include <QMap>

namespace nitro {

    class ImgResourceReader {

    public:
        static QPixmap
        getPixMap(const QString &path, const QSize &size = QSize(16, 16), const QColor &color = QColor(239, 239, 239));

    private:

        static QMap<QString, QPixmap> loadedIcons;
    };

} // nitro
