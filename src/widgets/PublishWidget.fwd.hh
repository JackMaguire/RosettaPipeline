#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>

#include <Options.hh>

namespace widgets {

class PublishWidget : public Wt::WContainerWidget, public OptionsHolder
{
public:
  PublishWidget( graph::GraphSP, OptionsSP );

  ~PublishWidget() override;
};


}//namespace widgets
