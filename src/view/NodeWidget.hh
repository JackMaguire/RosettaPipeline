#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Node.fwd.hh>

namespace view {

class NodeWidget : public Wt::WContainerWidget
{
public:
  NodeWidget( graph::NodeSP );

  virtual ~NodeWidget();

private:
  graph::NodeSP node_;
};


}//namespace view
