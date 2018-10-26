#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>

namespace view {

class SaveWidget : public Wt::WContainerWidget
{
public:
  SaveWidget(
    graph::GraphSP const &
  );

  virtual ~SaveWidget();
};


}//namespace view
