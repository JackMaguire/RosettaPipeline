#pragma once

#include <Wt/WContainerWidget.h>
#include <Wt/WTable.h>

#include <graph/Graph.fwd.hh>

#include <string>

namespace widgets {

class ExtraFileWidget : public Wt::WContainerWidget
{
public:
  ExtraFileWidget( graph::GraphSP graph );

  virtual ~ExtraFileWidget();

protected:
  void update_table();

private:
  graph::GraphSP graph_;
  Wt::WTable * table_;
};

}//namespace widgets
