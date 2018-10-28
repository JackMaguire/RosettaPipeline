#pragma once

#include <Wt/WContainerWidget.h>
#include <Wt/WHBoxLayout.h>

#include <view/graph_view.hh>

namespace view {

class TopWidget : public Wt::WContainerWidget
{
public:
  TopWidget( graph::GraphSP const & );

  virtual ~TopWidget();
};

}//namespace view
