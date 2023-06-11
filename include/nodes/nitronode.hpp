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
        // TODO: check lifetime of these references

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
        // when new widgets are added, they register a function that allows it to grab a value from the json list
        std::map<QString, std::function<void(QJsonValue)>> widgetsJson_;
        std::map<QString, QWidget *> widgets_;
        std::map<QString, int> options_; // used for dropdown

        QWidget *widget_ = nullptr;

        friend class NitroNodeBuilder;

        void connectInputWidget(ValueSliders::IntSlider *slider, int port);

        void connectInputWidget(ValueSliders::DoubleSlider *slider, int port);

        void connectLoadButton(QPushButton *button, int port);

        void connectComboBox(const QString &name, QComboBox *comboBox);

        void connectCheckBox(const QString &name, QCheckBox *checkBox);

        void loadImage(QPushButton *button, int port, const QString &filePath);

        void connectSourceInteger(ValueSliders::IntSlider *slider, int port);

        void connectSourceValue(ValueSliders::DoubleSlider *slider, int port);

        QString getInPortKey(unsigned int portIndex) const;

        QString getOutPortKey(unsigned int portIndex) const;
    };

} // nitro
