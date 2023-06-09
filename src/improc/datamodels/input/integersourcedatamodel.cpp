#include "integersourcedatamodel.hpp"
#include "external/nodeeditor/include/QtNodes/InvalidData.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QFileDialog>
#include <QImageReader>
#include <QVBoxLayout>
#include <QPainter>
#include "util/imgresourcereader.hpp"
#include "nodes/datatypes/integerdata.hpp"

nitro::IntegerSourceDataModel::IntegerSourceDataModel() {
}

QJsonObject nitro::IntegerSourceDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

    if (spinBox) {
        modelJson["value"] = spinBox->value();
    }
    return modelJson;
}

void nitro::IntegerSourceDataModel::load(QJsonObject const &p) {
    QJsonValue v = p["value"];

    if (!v.isUndefined()) {
        spinBox->setValue(v.toInt());
    }
}

unsigned int nitro::IntegerSourceDataModel::nPorts(QtNodes::PortType portType) const {
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
nitro::IntegerSourceDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex index) const {
    switch (portType) {
        case QtNodes::PortType::In:
            break;

        case QtNodes::PortType::Out:
            if (index == 0) {
                return IntegerData().type();
            }

        default:
            break;
    }
    return QtNodes::InvalidData().type();
}

std::shared_ptr<QtNodes::NodeData> nitro::IntegerSourceDataModel::outData(QtNodes::PortIndex) {
    return std::make_shared<IntegerData>(spinBox->value());
}

QWidget *nitro::IntegerSourceDataModel::embeddedWidget() {
    if(!_displayWrapper) {
        _displayWrapper = new QWidget();
        _displayWrapper->setAttribute(Qt::WA_TranslucentBackground);
        auto *layout = new QHBoxLayout(_displayWrapper);
        spinBox = new QSpinBox();
        spinBox->setMinimum(-10000);
        spinBox->setMaximum(10000);

        connect(spinBox, &QSpinBox::valueChanged, this, [this] {
            Q_EMIT dataUpdated(0);
        });
        layout->addWidget(spinBox);
        _displayWrapper->adjustSize();
    }
    return _displayWrapper;
}
