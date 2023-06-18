#include "stylepresets.hpp"

#include <QApplication>
#include <QToolTip>

void nitro::applyStylePreset(const StylePreset &preset) {
    QApplication::setPalette(preset.palette);
}

nitro::StylePreset nitro::getDarkModePalette() {
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(34, 34, 36));             // Window background color
    palette.setColor(QPalette::WindowText, QColor(239, 239, 239));      // Text color
    palette.setColor(QPalette::Base, QColor(43, 43, 45));               // Base color
    palette.setColor(QPalette::AlternateBase, QColor(42, 42, 43));      // Alternate base color
    palette.setColor(QPalette::ToolTipBase, QColor(34, 34, 36));     // ToolTip base color
    palette.setColor(QPalette::ToolTipText, QColor(189, 189, 189));           // ToolTip text color
    palette.setColor(QPalette::Text, QColor(239, 239, 239));            // Text color
    palette.setColor(QPalette::Button, QColor(35, 35, 40));             // Button color
    palette.setColor(QPalette::ButtonText, QColor(239, 239, 239));      // Button text color
    palette.setColor(QPalette::Highlight, QColor(71, 114, 179));        // Highlight color
    palette.setColor(QPalette::HighlightedText, QColor(239, 239, 239)); // Highlight text color
    palette.setColor(QPalette::PlaceholderText, QColor(150, 150, 150)); // PlaceHolder text color
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(160, 160, 160));
    palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(60, 60, 60));
    palette.setColor(QPalette::Disabled, QPalette::Button, QColor(60, 60, 60));
    palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(85, 86, 89));
    palette.setColor(QPalette::Disabled, QPalette::Base, QColor(42, 42, 46));
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase, QColor(45, 45, 48));
    palette.setColor(QPalette::Disabled, QPalette::Window, QColor(52, 52, 55));

    StylePreset preset;
    preset.palette = palette;
    return preset;
}

nitro::StylePreset nitro::getLightModePalette() {
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(222, 222, 222));           // Window background color
    palette.setColor(QPalette::WindowText, QColor(70, 70, 70));          // Text color
    palette.setColor(QPalette::Base, QColor(240, 240, 240));             // Base color
    palette.setColor(QPalette::AlternateBase, QColor(250, 250, 250));    // Alternate base color
    palette.setColor(QPalette::ToolTipBase, QColor(240, 240, 240));      // ToolTip base color
    palette.setColor(QPalette::ToolTipText, QColor(70, 70, 70));         // ToolTip text color
    palette.setColor(QPalette::Text, QColor(70, 70, 70));                // Text color
    palette.setColor(QPalette::Button, QColor(220, 220, 220));           // Button color
    palette.setColor(QPalette::ButtonText, QColor(70, 70, 70));          // Button text color
    palette.setColor(QPalette::Highlight, QColor(95, 130, 197));         // Highlight color
    palette.setColor(QPalette::HighlightedText, QColor(255, 255, 255));  // Highlight text color
    palette.setColor(QPalette::PlaceholderText, QColor(170, 170, 170));  // Placeholder text color
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(170, 170, 170));
    palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(170, 170, 170));
    palette.setColor(QPalette::Disabled, QPalette::Button, QColor(170, 170, 170));
    palette.setColor(QPalette::Disabled, QPalette::Highlight, Qt::lightGray);
    palette.setColor(QPalette::Disabled, QPalette::Base, Qt::white);
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase, QColor(238, 238, 238));
    palette.setColor(QPalette::Disabled, QPalette::Window, QColor(218, 218, 218));

    StylePreset preset;
    preset.palette = palette;

    return preset;
}
