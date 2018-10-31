#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <string>
//#include <cstdio> //std::tmpfile

namespace view {

class SaveWidget : public Wt::WContainerWidget
{
public:
  SaveWidget( graph::GraphSP );

  virtual ~SaveWidget();
};


}//namespace view
