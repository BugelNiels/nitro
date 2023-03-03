#include "progressupdater.h"

ProgressUpdater::ProgressUpdater() {}

ProgressUpdater::~ProgressUpdater() {}

void ProgressUpdater::notifyListeners() {
  for (auto& listener : listeners) {
    listener->progressUpdated(progress);
  }
}

void ProgressUpdater::addListener(ProgressListener* listener) {
  listeners.append(listener);
}

void ProgressUpdater::removeListener(ProgressListener* listener) {
  listeners.removeAll(listener);
}

void ProgressUpdater::resetProgress() {
  progress = 0;
  notifyListeners();
}
void ProgressUpdater::finalizeProgress() {
  progress = 100;
  notifyListeners();
}
void ProgressUpdater::setProgress(int value) {
  progress = value;
  notifyListeners();
}
