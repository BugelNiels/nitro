#pragma once

#include "3rdparty/nodeeditor/include/QtNodes/GraphicsView"
#include <QMenu>
#include "QtNodes/DataFlowGraphModel"

#include "src/gui/nodeeditor/nodegraphicsview.hpp"
#include "QtNodes/internal/AbstractNodeGeometry.hpp"
#include "ui/imgviewer/imgviewer.hpp"
#include "src/core/nodes/nodeinfo.hpp"

namespace nitro {

    class ImageNodeGraphicsView : public NodeGraphicsView {
    public:

        ImageNodeGraphicsView(nitro::ImageViewer *viewer, QtNodes::BasicGraphicsScene *scene,
                         QtNodes::DataFlowGraphModel *model,
                         QWidget *parent);

        ImageNodeGraphicsView(const GraphicsView &) = delete;


        NodeGraphicsView operator=(const GraphicsView &) = delete;


        void mousePressEvent(QMouseEvent *event) override;

        void mouseDoubleClickEvent(QMouseEvent *event) override;

        void setViewerNodeId(QtNodes::NodeId nodeId);

        QMenu *initNodeMenu();

    public Q_SLOTS:


    private:
        nitro::ImageViewer *_imViewer;
        QAction *spawnViewNodeAction = nullptr;


        QMenu *initInputSubMenu();

        QMenu *initConverterSubMenu();

        QMenu *initComparisonSubMenu();
        QMenu *initFilterSubMenu();
        QMenu *initColorSubMenu();

        QMenu *initOutputSubMenu();

        QtNodes::NodeId viewerNodeId;

        QAction *spawnViewerNodeAction();

        QMenu *initQuantizationSubMenu();

        QMenu *initResampleSubMenu();

        QtNodes::NodeId nodeIdViewed;
        QtNodes::PortIndex currentPort;

        QAction *spawnNodeAction(const nitro::NodeInfo& info);

        QtNodes::AbstractNodeGeometry &nodeGeometry;
    };

} // nitro
