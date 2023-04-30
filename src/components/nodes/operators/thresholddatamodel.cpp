#include "thresholddatamodel.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>


nitro::ThresholdDataModel::ThresholdDataModel() = default;

nitro::CbdImage nitro::ThresholdDataModel::compute(const nitro::CbdImage &inputImg) {

    int width = inputImg.width();
    int height = inputImg.height();
    nitro::CbdImage result(width, height, 2);

    bool lessThan = (_mode > 1);
    if (_mode == 1) {
        threshold -= 1;
    } else if (_mode == 3) {
        threshold += 1;
    }

    auto &inData = inputImg.constData();
    auto &outData = result.data();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int val = inData.get(x, y);
            bool satisfies = lessThan ? val < threshold : val > threshold;
            outData.set(x, y, satisfies ? 1 : 0);
        }
    }
    result.setIndexed({0, 255});
    return result;
}

void nitro::ThresholdDataModel::addWidgets(QLayout *layout) {
    auto *wrapper = new QWidget();
    auto *grid = new QGridLayout();
    grid->addWidget(new QLabel("Mode"), 1, 0);
    auto *combobox = new QComboBox();
    combobox->addItem(">");
    combobox->addItem(">=");
    combobox->addItem("<");
    combobox->addItem("<=");

    connect(combobox, SIGNAL (currentIndexChanged(int)), this, SLOT(modeChanged(int)));
    grid->addWidget(combobox, 1, 1);
    wrapper->setLayout(grid);
    layout->addWidget(wrapper);
}

void nitro::ThresholdDataModel::modeChanged(int mode) {
    _mode = mode;
    recompute();
}

