#pragma once

#include <QVector>

#include "progresslistener.hpp"

namespace nitro {

    class ProgressUpdater {
    public:
        ProgressUpdater();

        ~ProgressUpdater();

        void notifyListeners();

        void addListener(ProgressListener *listener);

        void removeListener(ProgressListener *listener);

        void resetProgress();

        void finalizeProgress();

        void setProgress(float value);

        void addProgress(float value);

    private:
        float progress{};
        QVector<ProgressListener *> listeners;
    };

}  // namespace nitro
