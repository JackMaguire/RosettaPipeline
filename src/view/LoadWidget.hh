#pragma once

#include <Options.hh>
#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <view/GraphWidget.fwd.hh>
#include <view/RightViewWidget.hh>

#include <string>

namespace view {

std::string load_file(
  std::string const & filename,
  graph::Graph & graph,
  Options & options
);

class LoadWidget : public Wt::WContainerWidget, OptionsHolder
{
public:
  LoadWidget(
    RightViewWidget * parent,
    graph::GraphSP const &,
    GraphWidget * graph_widget
  );

  ~LoadWidget() override;

private:
  RightViewWidget * parent_;
};

}//namespace view
