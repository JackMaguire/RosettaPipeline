#pragma once

#include <Wt/WTabWidget.h>
#include <graph/Graph.hh>
#include <view/GraphWidget.fwd.hh>

namespace view {

class RightViewWidget : public Wt::WTabWidget, graph::SelectionChangeListener
{
public:
  RightViewWidget(
    graph::GraphSP,
    GraphWidget * graph_widget
  );

  virtual ~RightViewWidget();

  void layoutSizeChanged( int w, int h ) override;

  void noteChangeInSelection() override;

private:
  graph::GraphSP graph_;
  int width_;
  int height_;
};

inline
void
RightViewWidget::layoutSizeChanged( int w, int h ) {
  width_ = w;
  height_ = h;
}


}//namespace view
