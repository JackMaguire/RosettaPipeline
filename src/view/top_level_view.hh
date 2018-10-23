#pragma once

#include <Wt/WContainerWidget>
#include <Wt/WHBoxLayout>

#include <view/graph_view.hh>

namespace view {

class TopWidget : public Wt::WPaintedWidget
{
public:
  TopWidget( Wt::WContainerWidget *root, graph::GraphSP const & );
  virtual ~TopWidget();
};


}//namespace view
