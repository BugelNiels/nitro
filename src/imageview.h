#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <resamplers/resampler.h>

#include <QLabel>
#include <QOpenGLDebugLogger>
#include <QOpenGLWidget>
#include <QScrollArea>
#include <QVector3D>

#include "progressupdater.h"
#include "settings/imviewsettings.h"
#include "settings/samplesettings.h"

class ImageView : public QScrollArea, public ProgressUpdater {
  Q_OBJECT

 public:
  ImageView(QWidget* parent = nullptr);
  ~ImageView() override;
  bool loadFile(const QString& fileName);
  inline const QImage& getOriginalImage() { return originalImg; }

  void quantisize();
  void calcDistanceField();
  void resample();

  const QImage& getActiveImage(bool display = false);

  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;

  void scaleImToFit();
  void resetImScale();
  void updateImage();

 private:
  // Probably extract this to own image class or something
  QImage quantisize(QImage& image, int numLevels);
  QVector<float**> calcDistanceField(QImage& image, bool use3D);
  QImage resample(QImage& image, int numLevels,
                  std::shared_ptr<Resampler> resampler);

  void setImage(const QImage& newImage);
  SampleSettings sampleSettings;
  ImViewSettings viewSettings;

  QImage originalImg;
  QMap<SampleMethod, QImage> resampledImages;
  QImage quantisizedImg;
  QImage displayQuantImg;

  QVector<float**> distanceField;
  QVector2D oldMouseCoords;
  QVector2D translation;
  QVector2D oldScrollValue;

  QLabel* imageLabel;
  float scale = 1.0;
  bool dragging;
  bool quantisized;

  // we make mainwindow a friend so it can access settings
  friend class MainWindow;
};

#endif  // IMAGE_VIEW_H
