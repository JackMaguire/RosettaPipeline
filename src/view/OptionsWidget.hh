#pragma once

#include <Wt/WContainerWidget.h>
#include <view/GraphWidget.fwd.hh>

namespace view {

class OptionsWidget : public Wt::WContainerWidget
{
public:
  OptionsWidget( GraphWidget * );

  virtual ~OptionsWidget();
};



}//namespace view
