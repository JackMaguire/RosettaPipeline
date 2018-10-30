#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <view/GraphWidget.fwd.hh>

namespace view {

class LeftViewWidget : public Wt::WContainerWidget
{
public:
  LeftViewWidget(
    graph::GraphSP
  );

  virtual ~LeftViewWidget();

  GraphWidget * graphWidget(){ return graph_widget_; }

private:
  GraphWidget * graph_widget_;
};



}//namespace view
