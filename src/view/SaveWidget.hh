#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <cstdio> //std::tmpfile

namespace view {

class SaveWidget : public Wt::WContainerWidget
{
public:
  SaveWidget(
    graph::GraphSP const &
  );

  virtual ~SaveWidget();

private:
  std::FILE * save_file_;
};


}//namespace view
