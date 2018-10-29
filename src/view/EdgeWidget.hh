#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Edge.fwd.hh>

namespace view {

class EdgeWidget : public Wt::WContainerWidget
{
public:
  EdgeWidget( graph::EdgeSP );

  virtual ~EdgeWidget();

private:
  graph::EdgeSP node_;
};


}//namespace view
