#pragma once

#include <QString>
#include "QtNodes/NodeDelegateModel"
#include "QtNodes/NodeInfo.hpp"
#include "nodeports.hpp"
#include "nodeoperator.hpp"

class QLabel;

class QPushButton;

class QComboBox;

class QCheckBox;

namespace ValueSliders {
    class IntSlider;

    class DoubleSlider;
}

namespace nitro {

    class NitroNode : public QtNodes::NodeDelegateModel {

    public:
        NitroNode() = default;

        ~NitroNode() override;

        void init(QtNodes::NodeInfo info,
                  const NodePorts &nodePorts,
                  std::unique_ptr<NodeOperator> algo,
                  QWidget *widget);

        [[nodiscard]] QString caption() const override;

        [[nodiscard]] bool captionVisible() const override;

        [[nodiscard]] QString name() const override;

        QWidget *embeddedWidget() override;

        [[nodiscard]] const QtNodes::NodeInfo &getInfo() const;

        [[nodiscard]] QJsonObject save() const override;

        void load(const QJsonObject &) override;

    public Q_SLOTS:

        void inputConnectionCreated(const QtNodes::ConnectionId &) override;
    void inputConnectionDeleted(const QtNodes::ConnectionId &) override;

    protected:

        [[nodiscard]] unsigned int nPorts(QtNodes::PortType portType) const override;

        [[nodiscard]] QtNodes::NodeDataType
        dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex) override;

    private:

        QtNodes::NodeInfo info_;
        std::unique_ptr<NodeOperator> algo_;
        NodePorts nodePorts_;
        QJsonObject propJson_;
        // when new widgets_ are added, they register a function that allows it to grab a value from the json list
        std::map<QString, std::function<void(QJsonValue)>> widgetsJson_;
        std::map<QString, QWidget *> widgets_;
        std::map<QString, int> options_; // used for dropdowns and other options

        QWidget *widget_ = nullptr;
        bool sliderInputDeleted_ = false;


        void connectInputWidget(ValueSliders::IntSlider *slider, QLabel* valLabel, int port);

        void connectInputWidget(ValueSliders::DoubleSlider *slider, QLabel* valLabel, int port);

        void connectLoadButton(const QString &name, QPushButton *button, int port, const QString &filter);

        void connectComboBox(const QString &name, QComboBox *comboBox);

        void connectCheckBox(const QString &name, QCheckBox *checkBox);

        void connectSourceInteger(ValueSliders::IntSlider *slider, int port);

        void connectSourceValue(ValueSliders::DoubleSlider *slider, int port);

        [[nodiscard]] QString getInPortKey(unsigned int portIndex) const;

        [[nodiscard]] QString getOutPortKey(unsigned int portIndex) const;

        void connectLabel(const QString &name, QLabel *label);

        friend class NitroNodeBuilder;


    };

} // nitro
