#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>

#include <Options.hh>

namespace view {

class PublishWidget : public Wt::WContainerWidget, public OptionsHolder
{
public:
  PublishWidget( graph::GraphSP, OptionsSP );

  ~PublishWidget() override;
};


}//namespace view
