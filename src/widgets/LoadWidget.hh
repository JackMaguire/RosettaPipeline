#pragma once

#include <Options.hh>
#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <widgets/GraphWidget.fwd.hh>
#include <widgets/RightViewWidget.hh>

#include <string>

namespace widgets {

class LoadWidget : public Wt::WContainerWidget, public OptionsHolder
{
public:
  LoadWidget(
    RightViewWidget * parent,
    graph::GraphSP const &,
    GraphWidget * graph_widget,
    OptionsSP
  );

  ~LoadWidget() override;

protected:
  void loadBrowseWidget();
private:
  RightViewWidget * parent_;
  GraphWidget * graph_widget_;
};

}//namespace widgets
