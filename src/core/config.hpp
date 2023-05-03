#pragma once

#include <Qt>
#include <QObject>

namespace nitro::config {
    class ConfigManager : public QObject {
    Q_OBJECT
    signals:
        void nodeImagesChanged();
    };

    extern bool nodeImages;
    extern ConfigManager configManager;

    void setNodeImages(bool newValue);

} // nitro
