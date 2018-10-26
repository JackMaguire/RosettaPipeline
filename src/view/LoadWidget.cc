#include <view/LoadWidget.hh>
#include <global_data/options.hh>

#include <graph/Graph.fwd.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>

#include <iostream>
#include <memory>

namespace view {

LoadWidget::LoadWidget(
  graph::GraphSP const & graph
) :
  WFileUpload(),
  width_( 500 ),
  height_( 800 )
{
  setLayoutSizeAware( true );
  resize( width_, height_ ); // Provide a default size.

  using namespace Wt;

}

LoadWidget::~LoadWidget(){}

}//namespace view
