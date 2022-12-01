#ifndef COLOR_SETTINGS_H
#define COLOR_SETTINGS_H

#include <QColor>
#include <QVector3D>

/*
 * Contains all settings related to colors.
 */
typedef struct ColorSettings {
  QVector3D background = QVector3D(0.19, 0.19, 0.2);
} ColorSettings;

QColor vecToCol(const QVector3D& col);
QVector3D colToVec(const QColor& col);

#endif  // COLOR_SETTINGS_H
