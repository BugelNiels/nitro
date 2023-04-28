#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <QPushButton>
#include <QLabel>

#include "cbdimage.hpp"
#include "nodes/imopdatamodel.hpp"

class QLineEdit;

namespace nitro {
    class ThresholdDataModel : public ImOpDataModel {
    Q_OBJECT

    public:
        ThresholdDataModel();

        virtual ~ThresholdDataModel() {}

    public:
        QString caption() const override { return QStringLiteral("Threshold"); }

        bool captionVisible() const override { return true; }

        QString name() const override { return QStringLiteral("Threshold"); }

    protected:
        nitro::CbdImage compute(const nitro::CbdImage& inputImg) override;

    private:
        int threshold = 128;
    };
}