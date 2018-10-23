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
  width_( 1000 ),
  height( 800 )
{
  setLayoutSizeAware( true );
  resize( width_, height_ ); // Provide a default size.

  using namespace Wt;

  WHBoxLayout * hbox = new WHBoxLayout();
  setLayout(hbox);

  GraphWidget * left_item = new GraphWidget( graph );
  hbox->addWidget( left_item, 0.4 );
}

TopWidget::~TopWidget(){}

}//namespace view
