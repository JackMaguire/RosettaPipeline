#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>

namespace view {

class ExamplesWidget : public Wt::WContainerWidget
{
public:
  ExamplesWidget( graph::GraphSP graph );

  virtual ~ExamplesWidget();
};


}//namespace view
