#pragma once

#include <Wt/WContainerWidget.h>
#include <view/GraphWidget.fwd.hh>
#include <view/RightViewWidget.hh>

namespace view {

class OptionsWidget : public Wt::WContainerWidget
{
public:
  OptionsWidget( RightViewWidget *, GraphWidget * );

  virtual ~OptionsWidget();

  void setNodeWidget( NodeWidget * );

protected:
  void setup_view_options( GraphWidget * );
  void setup_run_options();

  RightViewWidget * parent_;
};

}//namespace view
