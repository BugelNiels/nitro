#pragma once

#include <QDockWidget>
#include <QHBoxLayout>
#include <QDialog>

namespace nitro {

class MainWindow;

/**
 * @brief Dock widget used for custom GUI widgets. Provides basic functionality for constructing a (consistent) title bar.
 * Additionally, adds dock/undock functionality to the widget.
 */
class NitroDockWidget : public QDockWidget {
public:
    /**
     * @brief Creates a new dock widget.
     * @param title The title of the widget.
     * @param window The main window this dock should be added to.
     */
    NitroDockWidget(const QString &title, MainWindow *window);

    /**
     * @brief Destructor.
     */
    ~NitroDockWidget() override;

protected:
    /**
     * @brief Sets the icon of this dock widget.
     * @param path The path to the icon. Should be in the resources.
     */
    void setIcon(const QString& path);

    /**
     * @brief Adds a widget to the title bar.
     * @param widget The widget to add to the title bar.
     */
    void addTitleBarWidget(QWidget* widget);

    /**
     * @brief creates a simple widget that displays a "|" and adds some spacing around this.
     * Useful for visually grouping widgets in the title bar.
     * @return A simple QLabel.
     */
    [[nodiscard]] QWidget *spacing() const;

private:
    int wIdx = 2;
    bool docked = true;
    MainWindow *window_;
    std::unique_ptr<QDialog> dockFrame_ = nullptr;
    QString title_;
    QHBoxLayout *titleBarLayout_;

    QWidget *initTitleBarWidget();
};

} // namespace nitro
