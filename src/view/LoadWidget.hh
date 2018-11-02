#pragma once

//#include <Wt/WTabWidget.h>
#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <view/GraphWidget.fwd.hh>
#include <view/RightViewWidget.hh>

#include <string>

namespace view {

std::string load_file(
  std::string const & filename,
  graph::Graph & graph
);

class LoadWidget : public Wt::WContainerWidget
{
public:
  LoadWidget(
    RightViewWidget * parent,
    graph::GraphSP const &,
    GraphWidget * graph_widget
  );

  virtual ~LoadWidget();

  void layoutSizeChanged( int w, int h ) override;

private:
  int width_;
  int height_;

  RightViewWidget * parent_;
};

inline
void
LoadWidget::layoutSizeChanged( int w, int h ) {
  width_ = w;
  height_ = h;
}


}//namespace view
