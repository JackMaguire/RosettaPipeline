#include <view/TopWidget.hh>
#include <view/RightViewWidget.hh>
#include <view/GraphWidget.hh>

//#include <global_data/options.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WBorderLayout.h>

#include <iostream>
#include <math.h>
#include <memory>

namespace view {

TopWidget::TopWidget( graph::GraphSP const & graph ) :
  WContainerWidget(),
  width_( 1000 ),
  height_( 800 )
{
  setLayoutSizeAware( true );

  Wt::WHBoxLayout * const hbox =
    setLayout( Wt::cpp14::make_unique< Wt::WHBoxLayout >() );

  GraphWidget * const left_item =
    hbox->addWidget( Wt::cpp14::make_unique< GraphWidget >( graph ), 1 );

  Wt::WContainerWidget * const right_container =
    hbox->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), 2 );
  Wt::WBorderLayout * const right_layout =
    right_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
  right_layout->setSpacing( 0 );
  right_layout->addWidget( Wt::cpp14::make_unique< RightViewWidget >( graph, left_item ), Wt::LayoutPosition::Center );

}

TopWidget::~TopWidget(){}

}//namespace view
