#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Edge.fwd.hh>

namespace Wt{ class WVBoxLayout; }

namespace widgets {

class EdgeWidget : public Wt::WContainerWidget
{
public:
  EdgeWidget( graph::EdgeSP );

  virtual ~EdgeWidget();

protected:
  void add_intro( Wt::WVBoxLayout * outer_layout );
  void construct_segment1( Wt::WVBoxLayout * outer_layout );
  void construct_segment2( Wt::WVBoxLayout * outer_layout );
  void construct_segment3( Wt::WVBoxLayout * outer_layout );
  void construct_segment4( Wt::WVBoxLayout * outer_layout );
  void construct_dummy_segment( Wt::WVBoxLayout * outer_layout );

private:
  graph::EdgeSP edge_;
};


}//namespace widgets
