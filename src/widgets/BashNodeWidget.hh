#pragma once

#include <Wt/WContainerWidget.h>
#include <Wt/WVBoxLayout.h>

#include <graph/Node.fwd.hh>
#include <widgets/all_widgets.fwd.hh>

#include <Options.hh>

namespace widgets {

class BashNodeWidget : public Wt::WContainerWidget, public OptionsHolder
{
public:
  BashNodeWidget( graph::NodeSP, GraphWidget *, OptionsSP );

  virtual ~BashNodeWidget();

  void updateDefaultCommand( std::string const & );

private:
  graph::NodeSP node_;
};


}//namespace widgets
