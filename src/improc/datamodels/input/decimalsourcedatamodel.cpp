#include "decimalsourcedatamodel.hpp"
#include "src/core/invaliddata.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QPainter>
#include "util/imgresourcereader.hpp"
#include "datamodels/decimaldata.hpp"

nitro::DecimalSourceDataModel::DecimalSourceDataModel() {
}

QJsonObject nitro::DecimalSourceDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

    if (spinBox) {
        modelJson["value"] = spinBox->value();
    }
    return modelJson;
}

void nitro::DecimalSourceDataModel::load(QJsonObject const &p) {
    QJsonValue v = p["value"];

    if (!v.isUndefined()) {
        spinBox->setValue(v.toDouble());
    }
}

unsigned int nitro::DecimalSourceDataModel::nPorts(QtNodes::PortType portType) const {
    unsigned int result = 1;

    switch (portType) {
        case QtNodes::PortType::In:
            result = 0;
            break;

        case QtNodes::PortType::Out:
            result = 1;

        default:
            break;
    }

    return result;
}

QtNodes::NodeDataType
nitro::DecimalSourceDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {
    switch (portType) {
        case QtNodes::PortType::In:
            break;

        case QtNodes::PortType::Out:
            if (index == 0) {
                return DecimalData().type();
            }

        default:
            break;
    }
    return nitro::InvalidData().type();
}

std::shared_ptr<QtNodes::NodeData> nitro::DecimalSourceDataModel::outData(QtNodes::PortIndex) {
    return std::make_shared<DecimalData>(spinBox->value());
}

QWidget *nitro::DecimalSourceDataModel::embeddedWidget() {
    if(!_displayWrapper) {
        _displayWrapper = new QWidget();
        _displayWrapper->setAttribute(Qt::WA_TranslucentBackground);
        auto *layout = new QHBoxLayout(_displayWrapper);
        spinBox = new QDoubleSpinBox();
        spinBox->setMinimum(-10000);
        spinBox->setMaximum(10000);
        connect(spinBox, &QDoubleSpinBox::valueChanged, this, [this] {
            Q_EMIT dataUpdated(0);
        });
        layout->addWidget(spinBox);
        _displayWrapper->adjustSize();
    }
    return _displayWrapper;
}
