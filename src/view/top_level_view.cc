#include <view/top_level_view.hh>
#include <global_data/options.hh>

#include <graph/Graph.hh>
#include <graph/Edge.hh>
#include <graph/Node.hh>

#include <Wt/WLength>
#include <Wt/WGlobal>

#include <iostream>
#include <math.h>
#include <memory>

namespace view {

TopWidget::TopWidget(
  Wt::WContainerWidget * root,
  graph::GraphSP const & graph
) :
  WPaintedWidget( root ),
  graph_( graph ),
  width_( 1000 ),
  height( 800 )
{
  setLayoutSizeAware( true );
  resize( width_, height_ ); // Provide a default size.
}

TopWidget::~TopWidget(){}

}//namespace view
