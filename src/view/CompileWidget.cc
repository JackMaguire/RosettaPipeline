#include <view/CompileWidget.hh>
#include <global_data/options.hh>

#include <graph/Graph.fwd.hh>

#include <Wt/WLength>
#include <Wt/WGlobal>

#include <iostream>
#include <memory>

namespace view {

CompileWidget::CompileWidget(
  graph::GraphSP const & graph
) :
  WTabWidget( ),
  width_( 500 ),
  height_( 800 )
{
  setLayoutSizeAware( true );
  resize( width_, height_ ); // Provide a default size.

  using namespace Wt;

}

CompileWidget::~CompileWidget(){}

}//namespace view
