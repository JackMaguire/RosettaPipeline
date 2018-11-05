#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <Options.hh>

namespace view {

class CompileWidget : public Wt::WContainerWidget, OptionsHolder
{
public:
  CompileWidget( graph::GraphSP );

  ~CompileWidget() override;
};

}//namespace view
