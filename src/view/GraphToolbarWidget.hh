#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>

namespace view {

class GraphToolbarWidget : public Wt::WContainerWidget
{
public:
  GraphToolbarWidget( graph::GraphSP const & );

  virtual ~GraphToolbarWidget();
};


}//namespace view
