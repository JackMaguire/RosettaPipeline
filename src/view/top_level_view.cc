#include <view/top_level_view.hh>
#include <view/RightViewWidget.hh>
#include <global_data/options.hh>

#include <graph/Graph.hh>
#include <graph/Edge.hh>
#include <graph/Node.hh>

#include <Wt2/WLength.h>
#include <Wt2/WGlobal.h>

#include <iostream>
#include <math.h>
#include <memory>

namespace view {

TopWidget::TopWidget(
  graph::GraphSP const & graph
) :
  WContainerWidget(),
  width_( 1000 ),
  height_( 800 )
{
  setLayoutSizeAware( true );
  resize( width_, height_ ); // Provide a default size.

  using namespace Wt;

  auto hbox = setLayout( Wt::cpp14::make_unique< Wt::WHBoxLayout >() );
  /*WHBoxLayout * hbox = new WHBoxLayout();
  setLayout(hbox);*/

  //GraphWidget * left_item = new GraphWidget( graph );
  //hbox->addWidget( left_item, 0.4 );
  hbox->addWidget( Wt::cpp14::make_unique< GraphWidget >( graph ) );

  //RightViewWidget * right_item = new RightViewWidget( graph );
  //hbox->addWidget( right_item, 0.6 );
  hbox->addWidget( Wt::cpp14::make_unique< RightViewWidget >( graph ) );

}

TopWidget::~TopWidget(){}

}//namespace view
