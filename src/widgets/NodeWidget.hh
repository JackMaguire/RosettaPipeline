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

protected:
  void construct_segment1( GraphWidget *, Wt::WVBoxLayout & layout );

  void construct_segment2( Wt::WVBoxLayout & layout );

  void construct_segment3( Wt::WVBoxLayout & layout );

  void construct_segment4( Wt::WVBoxLayout & layout );

private:
  graph::NodeSP node_;
  Wt::WLineEdit * command_edit_;
};


}//namespace widgets
