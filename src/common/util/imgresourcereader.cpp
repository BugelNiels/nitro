#include "imgresourcereader.hpp"
#include <QPainter>


QMap<QString, QPixmap> nitro::ImgResourceReader::loadedIcons;


QPixmap nitro::ImgResourceReader::getPixMap(const QString &path, const QSize &size, const QColor &color) {
    QString key = QString("%1?color=%2&size=%3x%4").arg(path).arg(color.name()).arg(size.width()).arg(
            size.height());
    if (loadedIcons.contains(key)) {
        return ImgResourceReader::loadedIcons[key];
    }
    QImage myImage(path);

    // TODO: check if this can be done faster.
    QSize size2 = myImage.size();

    for (int x = 0; x < size2.width(); x++) {
        for (int y = 0; y < size2.height(); y++) {
            if (myImage.pixelColor(x, y).alpha() != 0) {
                myImage.setPixelColor(x, y, color);
            }
        }
    }

    auto newIcon = QPixmap::fromImage(myImage).scaled(size, Qt::AspectRatioMode::KeepAspectRatio);
    loadedIcons[key] = newIcon;
    return newIcon;
}