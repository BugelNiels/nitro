#ifndef PROGRESSUPDATER_H
#define PROGRESSUPDATER_H

#include <QVector>

#include "ProgressListener.h"

class ProgressUpdater {
 public:
  ProgressUpdater();
  ~ProgressUpdater();

  void notifyListeners();
  void addListener(ProgressListener* listener);
  void removeListener(ProgressListener* listener);

 protected:
  void resetProgress();
  void finalizeProgress();
  void setProgress(int value);

 private:
  int progress;
  QVector<ProgressListener*> listeners;
};

#endif  // PROGRESSUPDATER_H
