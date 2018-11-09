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
    graph::GraphSP graph,
    OptionsSP
  );

  ~ExamplesWidget() override;
};


}//namespace widgets
