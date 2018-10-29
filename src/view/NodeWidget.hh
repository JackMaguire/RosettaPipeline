#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Node.fwd.hh>
#include <view/GraphWidget.fwd.hh>

namespace view {

class NodeWidget : public Wt::WContainerWidget
{
public:
  NodeWidget( graph::NodeSP, GraphWidget * );

  virtual ~NodeWidget();

protected:
  void contruct_segment1( GraphWidget * );

private:
  graph::NodeSP node_;
};


}//namespace view
