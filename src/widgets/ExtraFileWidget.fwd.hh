#pragma once

#include <Wt/WContainerWidget.h>
#include <Wt/WTable.h>

#include <graph/Graph.fwd.hh>
#include <refresh.hh>

#include <string>
//#include <iostream>

namespace widgets {

class ExtraFileWidget : public Wt::WContainerWidget, public Refreshable
{
public:
  ExtraFileWidget( graph::GraphSP graph );

  virtual ~ExtraFileWidget();

  void update_table();

  void refresh() override {
    //std::cout << "refreshing" << std::endl;
    update_table();
  }

private:
  graph::GraphSP graph_;
  Wt::WTable * table_;
};

}//namespace widgets
