#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <Options.hh>
#include <refresh.hh>

namespace widgets {

class WelcomeWidget : public Wt::WContainerWidget
{
public:
  WelcomeWidget(
    //graph::GraphSP graph,
    //OptionsSP,
    //RefreshableElementVecSP refreshers
  );

  virtual ~WelcomeWidget();
};


}//namespace widgets
