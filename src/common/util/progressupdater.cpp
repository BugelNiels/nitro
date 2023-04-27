#include "progressupdater.hpp"

nitro::ProgressUpdater::ProgressUpdater() = default;

nitro::ProgressUpdater::~ProgressUpdater() = default;

void nitro::ProgressUpdater::notifyListeners() {
    for (auto &listener: listeners) {
        listener->progressUpdated(progress);
    }
}

void nitro::ProgressUpdater::addListener(ProgressListener *listener) {
    listeners.append(listener);
}

void nitro::ProgressUpdater::removeListener(ProgressListener *listener) {
    listeners.removeAll(listener);
}

void nitro::ProgressUpdater::resetProgress() {
    progress = 0;
    notifyListeners();
}

void nitro::ProgressUpdater::finalizeProgress() {
    progress = 100;
    notifyListeners();
}

void nitro::ProgressUpdater::addProgress(float value) {
#pragma atomic add
    progress += value;
    notifyListeners();
}

void nitro::ProgressUpdater::setProgress(float value) {
#pragma atomic
    progress = value;
    notifyListeners();
}
