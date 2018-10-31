#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>

namespace view {

class CompileWidget : public Wt::WContainerWidget
{
public:
  CompileWidget( graph::GraphSP );

  virtual ~CompileWidget();
};

}//namespace view
