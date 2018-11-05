#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <Options.hh>

namespace view {

class CompileWidget : public Wt::WContainerWidget, public OptionsHolder
{
public:
  CompileWidget( graph::GraphSP, OptionsSP );

  ~CompileWidget() override;
};

}//namespace view
