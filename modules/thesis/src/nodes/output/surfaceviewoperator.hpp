#pragma once

#include "gui/im3dviewer/renderview.hpp"
#include "gui/mainwindow.hpp"
#include <nodes/nitronode.hpp>
#include <nodes/nodeoperator.hpp>

namespace nitro::Thesis {

class SurfaceViewOperator : public NodeOperator {

public:
    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category,
                                                               MainWindow *window);

    explicit SurfaceViewOperator(MainWindow *window);

    ~SurfaceViewOperator();

    void execute(NodePorts &nodePorts) override;

private:
    MainWindow *window_;
    QDockWidget *dockWidget_;
    RenderView *surfViewer_;
    cv::Mat currentImg_;
};
} // namespace nitro::Thesis
