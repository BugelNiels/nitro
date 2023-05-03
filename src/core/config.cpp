#include "config.hpp"

namespace nitro::config {
    ConfigManager configManager;
    bool nodeImages = true;

    void setNodeImages(bool newValue) {
        if (nodeImages != newValue) {
            nodeImages = newValue;
            configManager.nodeImagesChanged();
        }
    }

} // nitro