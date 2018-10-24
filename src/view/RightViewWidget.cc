#include <view/RightViewWidget.hh>

#include <view/EditWidget.hh>
#include <view/CompileWidget.hh>

#include <global_data/options.hh>

#include <graph/Graph.fwd.hh>

#include <Wt2/WLength.h>
#include <Wt2/WGlobal.h>

#include <iostream>
#include <memory>

namespace view {

RightViewWidget::RightViewWidget(
  graph::GraphSP const & graph
) :
  WTabWidget( root ),
  width_( 500 ),
  height_( 800 )
{
  setLayoutSizeAware( true );
  resize( width_, height_ ); // Provide a default size.

  using namespace Wt;

  addTab( new EditWidget( graph ), "Edit" );
  addTab( new CompileWidget( graph ), "Compile" );
}

RightViewWidget::~RightViewWidget(){}

}//namespace view
