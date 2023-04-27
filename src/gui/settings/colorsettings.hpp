#pragma once

#include <QColor>
#include <QVector3D>

namespace nitro {

/*
 * Contains all settings related to colors.
 */
typedef struct ColorSettings {
  QVector3D background = QVector3D(0.19, 0.19, 0.2);
} ColorSettings;

QColor vecToCol(const QVector3D &col);
QVector3D colToVec(const QColor &col);

}  // namespace nitro
