#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <string>
//#include <cstdio> //std::tmpfile

namespace view {

class NodeWidget : public Wt::WContainerWidget
{
public:
  NodeWidget(
    graph::GraphSP const &
  );

  virtual ~NodeWidget();

private:
  std::string save_filename_;
};


}//namespace view
