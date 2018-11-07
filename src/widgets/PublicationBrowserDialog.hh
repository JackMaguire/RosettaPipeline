#pragma once

#include <Wt/WDialog.h>
#include <Wt/WTable.h>
#include <Wt/WContainerWidget.h>

#include <Options.hh>
#include <graph/Graph.fwd.hh>
#include <publishing/load.hh>
#include <widgets/GraphWidget.hh>
#include <widgets/OptionsWidget.hh>

#include <list>

namespace widgets {

class PublicationBrowserDialog : public Wt::WDialog
{
public:
  PublicationBrowserDialog(
    Wt::WContainerWidget * parent,
    GraphWidget * graph_widget,
    OptionsWidget * options_widget,
    graph::GraphSP graph,
    OptionsSP options
  );

  virtual ~PublicationBrowserDialog();

  void reset_table( std::list< publishing::Publication > const & new_elements );

private:
  Wt::WContainerWidget * parent_;
  graph::GraphSP graph_;
  OptionsSP options_;
  Wt::WTable * table_;
  GraphWidget * graph_widget_;
  OptionsWidget * options_widget_;
};

}//namespace widgets
