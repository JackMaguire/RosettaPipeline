#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <view/GraphWidget.fwd.hh>
#include <view/RightViewWidget.hh>

namespace view {

class ExamplesWidget : public Wt::WContainerWidget
{
public:
  ExamplesWidget(
    RightViewWidget * parent,
    graph::GraphSP graph,
    GraphWidget * graph_widget
  );

  virtual ~ExamplesWidget();

private:
  RightViewWidget * parent_;
};


}//namespace view
