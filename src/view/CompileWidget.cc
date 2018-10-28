#include <view/CompileWidget.hh>
#include <global_data/options.hh>

#include <graph/Graph.fwd.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>

#include <iostream>
#include <memory>

namespace view {

CompileWidget::CompileWidget(
  graph::GraphSP const & graph
) :
  WContainerWidget( )
{
}

CompileWidget::~CompileWidget(){}

}//namespace view
