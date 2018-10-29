#include <view/TopWidget.hh>
#include <view/RightViewWidget.hh>
#include <view/GraphWidget.hh>

//#include <global_data/options.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WHBoxLayout.h>

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

  auto hbox = setLayout( Wt::cpp14::make_unique< Wt::WHBoxLayout >() );

  GraphWidget * left_item =
    hbox->addWidget( Wt::cpp14::make_unique< GraphWidget >( graph ), 0.3 );

  hbox->addWidget( Wt::cpp14::make_unique< RightViewWidget >( graph, left_item ), 0.7 );

}

TopWidget::~TopWidget(){}

}//namespace view
