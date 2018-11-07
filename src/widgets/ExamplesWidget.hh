#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <widgets/GraphWidget.fwd.hh>
//#include <widgets/RightViewWidget.hh>

#include <Options.hh>

namespace widgets {

class RightViewWidget;

class ExamplesWidget : public Wt::WContainerWidget, public OptionsHolder
{
public:
  ExamplesWidget(
    RightViewWidget * parent,
    graph::GraphSP graph,
    GraphWidget * graph_widget,
    OptionsSP
  );

  ~ExamplesWidget() override;

private:
  RightViewWidget * parent_;
};


}//namespace widgets
