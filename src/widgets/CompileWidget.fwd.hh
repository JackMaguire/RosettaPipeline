#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <Options.hh>

namespace widgets {

class CompileWidget : public Wt::WContainerWidget, public OptionsHolder
{
public:
  CompileWidget( graph::GraphSP, OptionsSP );

  ~CompileWidget() override;
};

}//namespace widgets
