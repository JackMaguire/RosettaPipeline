#include <view/RightViewWidget.hh>

#include <view/EditWidget.hh>
#include <view/CompileWidget.hh>

#include <global_data/options.hh>

#include <graph/Graph.fwd.hh>

#include <Wt/WLength>
#include <Wt/WGlobal>

#include <iostream>
#include <memory>

namespace view {

RightViewWidget::RightViewWidget(
  Wt::WContainerWidget * root,
  graph::GraphSP const & graph
) :
  WTabWidget( root ),
  width_( 500 ),
  height_( 800 )
{
  setLayoutSizeAware( true );
  resize( width_, height_ ); // Provide a default size.

  using namespace Wt;

  addTab( new EditWidget( this, graph ), "Edit" );
  addTab( new CompileWidget( this, graph ), "Compile" );
}

RightViewWidget::~RightViewWidget(){}

}//namespace view
