#include "thresholddatamodel.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>


nitro::ThresholdDataModel::ThresholdDataModel() = default;

void nitro::ThresholdDataModel::modeChanged(int mode) {
    _mode = mode;
    recompute();
}

void nitro::ThresholdDataModel::thresholdValChanged(int val) {
    threshold = val;
    recompute();
}

QWidget *nitro::ThresholdDataModel::initBeforeWidget() {
    auto *wrapper = new QWidget();
    auto *horLayout = new QHBoxLayout();

    auto *combobox = new QComboBox();
    combobox->addItem(">");
    combobox->addItem(">=");
    combobox->addItem("<");
    combobox->addItem("<=");
    connect(combobox, SIGNAL (currentIndexChanged(int)), this, SLOT(modeChanged(int)));
    horLayout->addWidget(combobox);

    auto *spinBox = new QSpinBox();
    spinBox->setMinimum(0);
    spinBox->setMaximum(255); // TODO: update based on levels of input
    spinBox->setValue(threshold);
    connect(spinBox, SIGNAL (valueChanged(int)), this, SLOT(thresholdValChanged(int)));
    horLayout->addWidget(spinBox);

    wrapper->setLayout(horLayout);
    return wrapper;
}

QWidget *nitro::ThresholdDataModel::initAfterWidget() {
    return nullptr;
}

std::shared_ptr<nitro::ImageData> nitro::ThresholdDataModel::compute(const QImage &inputImg) {
    return compute(nitro::CbdImage(inputImg.convertToFormat(QImage::Format_Grayscale8)));
}

std::shared_ptr<nitro::ImageData> nitro::ThresholdDataModel::compute(const nitro::CbdImage &inputImg) {
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
    return std::make_shared<nitro::ImageData>(result);
}


