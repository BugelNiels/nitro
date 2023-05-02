#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>

#include "cbdimage.hpp"
#include "src/nodes/operators/imopdatamodel.hpp"

class QLineEdit;

namespace nitro {

    enum ImgMathMode {MULTIPLY, DIVIDE, ADD, SUBTRACT};

    class ImgMathDataModel : public ImOpDataModel {
    Q_OBJECT

    public:
        ImgMathDataModel();

        virtual ~ImgMathDataModel() {}

    public:
        static QString nodeCaption() { return QStringLiteral("Math"); }

        static QString nodeName() { return QStringLiteral("Math"); }
        static QString nodeIcon() { return QStringLiteral(":/icons/nodes/math.png"); }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }


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

        QWidget *initAfterWidget() override;

    private:
        double mathVal = 1.0;

        QComboBox *modeCombobox;
        QDoubleSpinBox *valSpinBox;
    };
}