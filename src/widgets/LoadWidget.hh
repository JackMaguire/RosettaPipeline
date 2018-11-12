#pragma once

#include <Options.hh>
#include <refresh.hh>

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
    graph::GraphSP,
    OptionsSP,
    RefreshableElementVecSP
  );

  ~LoadWidget() override;

protected:
  void loadBrowseWidget();
private:
  RightViewWidget * parent_;
  graph::GraphSP graph_;
  RefreshableElementVecSP refreshers_;
};

}//namespace widgets
