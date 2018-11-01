#pragma once

#include <Wt/WContainerWidget.h>

namespace view {

class ExamplesWidget : public Wt::WContainerWidget
{
public:
  ExamplesWidget( graph::GraphSP graph );

  virtual ~ExamplesWidget();
};


}//namespace view
