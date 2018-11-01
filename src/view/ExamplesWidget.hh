#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <view/GraphWidget.fwd.hh>

namespace view {

class ExamplesWidget : public Wt::WContainerWidget
{
public:
  ExamplesWidget( graph::GraphSP graph, GraphWidget * graph_widget );

  virtual ~ExamplesWidget();
};


}//namespace view
