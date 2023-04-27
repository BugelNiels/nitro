#pragma once

#include <QDockWidget>

class NodeView : public QDockWidget {
 public:
  explicit NodeView(QWidget *parent = nullptr);

  ~NodeView() override;

 private:
};
