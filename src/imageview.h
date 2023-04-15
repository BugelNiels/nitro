#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <resamplers/resampler.h>

#include <QLabel>
#include <QOpenGLDebugLogger>
#include <QOpenGLWidget>
#include <QScrollArea>
#include <QVector3D>

#include "matrix.h"
#include "progressupdater.h"
#include "settings/imviewsettings.h"
#include "settings/samplesettings.h"

#define MIN(A, B) (A) < (B) ? (A) : (B)
#define MAX(A, B) (A) > (B) ? (A) : (B)

class ImageView : public QScrollArea, public ProgressUpdater {
  Q_OBJECT

 public:
  ImageView(QWidget* parent = nullptr);
  ~ImageView() override;
  bool loadFile(const QString& fileName);
  inline const QImage& getOriginalImage() { return originalImg; }
  inline const QImage& getQuantisizedImage() { return quantisizedImg; }

  const QImage getImageByIndex(int index);

  void quantisize();
  void calcDistanceField();
  void resample();
  void compressDecompress();

  const QImage& getActiveImage(bool display = false);
  const QImage& getActiveDisplayImage();

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
  QVector<Matrix<float>> calcDistanceField(QImage& image,
                                           double pixelMult = -1);
  QImage resample(QImage& image, int numLevels,
                  std::shared_ptr<Resampler> resampler);

  QImage compress(QImage& img, int targetBitDepth);

  QImage decompress(QImage& img, int targetBitDepth);

  void setImage(const QImage& newImage);
  SampleSettings sampleSettings;
  ImViewSettings viewSettings;

  QImage originalImg;
  QImage quantisizedImg;

  QVector<Matrix<float>> distanceField;
  QVector2D oldMouseCoords;
  QVector2D translation;
  QVector2D oldScrollValue;

  QLabel* imageLabel;
  float scale = 1.0;
  bool dragging;
  bool quantisized;

  QList<QImage> savedImages;

  // we make mainwindow a friend so it can access settings
  friend class MainWindow;
};

#endif  // IMAGE_VIEW_H
