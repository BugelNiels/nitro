#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>

#include "src/core/cbdimage.hpp"
#include "datamodels/operators/imopdatamodel.hpp"

class QLineEdit;

namespace nitro {

    enum ImgMathMode {MULTIPLY, DIVIDE, ADD, SUBTRACT};

    class ImgMathDataModel : public ImOpDataModel {
    Q_OBJECT

    public:
        ImgMathDataModel();

        virtual ~ImgMathDataModel() {}

    public:

        static QtNodes::NodeInfo nodeInfo() {
            return {"Math",
                    "Math",
                    {36, 98, 131},
                    ":/icons/nodes/math.png"};
        }

        QString caption() const override { return nodeInfo().getNodeName(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeInfo().getNodeId(); }

    public:
        QJsonObject save() const override;

        void load(QJsonObject const &p) override;

    public Q_SLOTS:

        void modeChanged(int mode);

        void mathValChanged();

    protected:
        ImgMathMode _mode = ImgMathMode::MULTIPLY;

        std::shared_ptr<ImageData> compute(const QImage &inputImg) override;

        std::shared_ptr<ImageData> compute(const nitro::CbdImage &inputImg) override;

        QWidget *initBeforeWidget() override;

    private:
        double mathVal = 1.0;

        QComboBox *modeCombobox;
        QDoubleSpinBox *valSpinBox;
    };
}