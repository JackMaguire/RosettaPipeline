#pragma once

#include <Options.hh>
#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <widgets/RightViewWidget.hh>

#include <string>

namespace widgets {

class LoadWidget : public Wt::WContainerWidget, public OptionsHolder
{
public:
  LoadWidget(
    RightViewWidget * parent,
    graph::GraphSP const &,
    OptionsSP
  );

  ~LoadWidget() override;

protected:
  void loadBrowseWidget();
private:
  RightViewWidget * parent_;
  graph::GraphSP graph_;
};

}//namespace widgets
