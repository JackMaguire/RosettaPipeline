#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>

#include <Options.hh>

namespace view {

class SaveWidget : public Wt::WContainerWidget, public OptionsHolder
{
public:
  SaveWidget( graph::GraphSP, OptionsSP );

  ~SaveWidget() override;
};


}//namespace view
