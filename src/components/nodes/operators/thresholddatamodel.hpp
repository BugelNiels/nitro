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

    public:static QString nodeCaption() { return QStringLiteral("Threshold"); }
        static QString nodeName() { return QStringLiteral("Threshold"); }

        QString caption() const override { return nodeCaption(); }

        bool captionVisible() const override { return true; }

        QString name() const override { return nodeName(); }

    protected:
        nitro::CbdImage compute(const nitro::CbdImage& inputImg) override;

    private:
        int threshold = 128;
    };
}