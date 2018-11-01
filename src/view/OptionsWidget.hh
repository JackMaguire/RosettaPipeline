#pragma once

#include <Wt/WContainerWidget.h>
#include <view/GraphWidget.fwd.hh>
#include <view/NodeWidget.hh>

namespace view {

class OptionsWidget : public Wt::WContainerWidget
{
public:
  OptionsWidget( GraphWidget * );

  virtual ~OptionsWidget();

  void setNodeWidget( NodeWidget * );

protected:
  void setup_view_options( GraphWidget * );
  void setup_run_options();

  NodeWidget * current_node_widget_;//Sad that wt does not support shared_ptr
};

inline
void
OptionsWidget::setNodeWidget( NodeWidget * node ) {
  current_node_widget_ = node;
}

}//namespace view
