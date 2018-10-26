#pragma once

#include <Wt/WTabWidget.h>
#include <graph/Graph.fwd.hh>
#include <view/graph_view.fwd.hh>

namespace view {

class RightViewWidget : public Wt::WTabWidget
{
public:
  RightViewWidget(
    graph::GraphSP const &,
    GraphWidget * graph_widget
  );

  virtual ~RightViewWidget();

  void layoutSizeChanged( int w, int h ) override;

private:
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
