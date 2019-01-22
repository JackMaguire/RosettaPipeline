#pragma once

#include <Wt/WContainerWidget.h>
#include <Wt/WVBoxLayout.h>

#include <graph/Node.fwd.hh>
#include <widgets/all_widgets.fwd.hh>

#include <Options.hh>

namespace widgets {

class NodeWidget : public Wt::WContainerWidget, public OptionsHolder
{
public:
  NodeWidget( graph::NodeSP, GraphWidget *, OptionsSP );

  virtual ~NodeWidget();

  void updateDefaultCommand( std::string const & );

private:
  graph::NodeSP node_;
  RSNodeWidget * rs_node_widget_ = 0;
  //BashNodeWidget * bash_node_widget_ = 0;
};


}//namespace widgets
