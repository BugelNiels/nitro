#pragma once

#include <QIcon>
#include <QMap>

namespace nitro {

/**
 * Responsible for reading Pixmaps for the icons.
 */
class ImResourceReader {

public:
    /**
     * Retrieves a Pixmap of the icon at the provided path.
     * @param path The path of the icon. Should be present in (one of) the resource files.
     * @param size The size to load the icon at. Defaults to 42x42.
     * @param color The color to load the icon as. Defaults to a light grey.
     * @return The Pixmap of the icon with the provided size and color.
     */
    static QPixmap getPixMap(const QString &path,
                             const QSize &size = QSize(42, 42),
                             const QColor &color = QColor(180, 180, 180));

private:
    static QMap<QString, QPixmap> loadedIcons_;
};

} // namespace nitro
