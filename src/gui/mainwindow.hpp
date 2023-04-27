#pragma once

#include <QMainWindow>

#include "imageview.hpp"
#include "renderview.hpp"
#include "util/progresslistener.hpp"

namespace Ui {
    class MainWindow;
}

namespace nitro {

/**
 * @brief
 *
 */
    class MainWindow : public QMainWindow, public ProgressListener {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

    private slots:

        void on_loadImageButton_pressed();

        void on_quantisizeLevelSpinBox_valueChanged(int arg1);

        void on_sampleButton_pressed();

        void on_methodComboBox_currentIndexChanged(int index);

        void on_compareButton_pressed();

        void on_compareButton_released();

        void on_fitButton_clicked();

        void on_resetButton_clicked();

        void on_quantisizeButton_clicked();

        void on_computeSDFButton_pressed();

        void progressUpdated(float value) override;

        void on_savedImagesView_itemSelectionChanged();

        void on_compareImagesView_itemSelectionChanged();

        void on_compressButton_pressed();

        void on_ditheringCheckBox_toggled(bool checked);

        void on_quantMethodComboBox_currentIndexChanged(int index);

        void on_differencePushButton_pressed();

    private:
        void newImage(const QString &name);

        int oldShowIdx;

        void updateRenderImage();

        void updateNumLevels(int value);

        Ui::MainWindow *ui;
    };
}  // namespace nitro
