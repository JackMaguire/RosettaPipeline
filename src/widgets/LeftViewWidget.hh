#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <widgets/all_widgets.fwd.hh>
#include <Options.hh>

namespace widgets {

class LeftViewWidget : public Wt::WContainerWidget
{
public:
  LeftViewWidget(
    graph::GraphSP,
    OptionsSP
  );

  ~LeftViewWidget() override;

  GraphWidget * graphWidget(){ return graph_widget_; }

private:
  GraphWidget * graph_widget_;
};



}//namespace widgets
