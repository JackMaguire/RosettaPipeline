#pragma once

//#include <Wt/WTabWidget.h>
#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <view/graph_view.fwd.hh>

namespace view {

class LoadWidget : public Wt::WContainerWidget
{
public:
  LoadWidget(
    graph::GraphSP const &,
    GraphWidget * graph_widget
  );

  virtual ~LoadWidget();

  void layoutSizeChanged( int w, int h ) override;

private:
  int width_;
  int height_;
};

inline
void
LoadWidget::layoutSizeChanged( int w, int h ) {
  width_ = w;
  height_ = h;
}


}//namespace view
