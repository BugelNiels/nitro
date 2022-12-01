#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <QLabel>
#include <QOpenGLDebugLogger>
#include <QOpenGLWidget>
#include <QScrollArea>
#include <QVector3D>

class ImageView : public QScrollArea {
  Q_OBJECT

 public:
  ImageView(QWidget* parent = nullptr);
  ~ImageView() override;
  void loadFile(const QString& fileName);

 private:
  void setImage(const QImage& newImage);

  QImage image;
  QLabel* imageLabel;
  double scaleFactor = 1;
};

#endif  // IMAGE_VIEW_H
