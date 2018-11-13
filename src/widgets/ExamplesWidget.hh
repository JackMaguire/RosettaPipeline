#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <widgets/all_widgets.fwd.hh>
//#include <widgets/RightViewWidget.hh>

#include <Options.hh>
#include <refresh.hh>

#include <functional>

namespace widgets {

class RightViewWidget;

class ExamplesWidget : public Wt::WContainerWidget, public OptionsHolder
{
public:
  ExamplesWidget(
    graph::GraphSP graph,
    OptionsSP,
    RefreshableElementVecSP refreshers
  );

  ~ExamplesWidget() override;

  template< typename T >
  void set_action_to_call_upon_load( T action ){
    action_to_call_upon_load_ = action;
  }

private:
  std::function< void() > action_to_call_upon_load_;
};


}//namespace widgets
