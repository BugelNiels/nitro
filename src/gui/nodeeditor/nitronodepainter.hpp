#pragma once

#include <QTreeWidgetItem>
#include <QMap>
#include <QtNodes/internal/DefaultNodePainter.hpp>

namespace nitro {

class NitroNodePainter : public QtNodes::AbstractNodePainter {
    public:
        NitroNodePainter();

    void paint(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const override;
    void drawNodeBackground(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const;
    void drawNodeHighlight(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const;

    void drawConnectionPoints(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const;

    void drawFilledConnectionPoints(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const;

    void drawNodeCaption(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const;

    void drawEntryLabels(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const;

    void drawResizeRect(QPainter *painter, QtNodes::NodeGraphicsObject &ngo) const;
};


} // nitro
