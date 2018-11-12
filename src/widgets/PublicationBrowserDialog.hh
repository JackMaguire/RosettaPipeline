#pragma once

#include <Wt/WDialog.h>
#include <Wt/WTable.h>
#include <Wt/WContainerWidget.h>

#include <Options.hh>
#include <graph/Graph.fwd.hh>
#include <publishing.hh>
#include <widgets/GraphWidget.hh>
#include <widgets/OptionsWidget.hh>

#include <list>

namespace widgets {

class PublicationBrowserDialog : public Wt::WDialog
{
public:
  PublicationBrowserDialog(
    Wt::WContainerWidget * parent,
    graph::GraphSP graph,
    OptionsSP options,
    RefreshableElementVecSP
  );

  virtual ~PublicationBrowserDialog();

  void reset_table( std::list< publishing::Publication > const & new_elements );

private:
  Wt::WContainerWidget * parent_;
  graph::GraphSP graph_;
  OptionsSP options_;
  RefreshableElementVecSP refreshers_;
  Wt::WTable * table_;
};

}//namespace widgets
