#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <string>
//#include <cstdio> //std::tmpfile

namespace view {

class EdgeWidget : public Wt::WContainerWidget
{
public:
  EdgeWidget(
    graph::GraphSP const &
  );

  virtual ~EdgeWidget();

private:
  std::string save_filename_;
};


}//namespace view
