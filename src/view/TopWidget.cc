#include <view/TopWidget.hh>
#include <view/RightViewWidget.hh>
#include <view/LeftViewWidget.hh>
#include <view/GraphWidget.hh>
#include <view/GraphToolbarWidget.hh>

#include <Wt/WLength.h>
#include <Wt/WImage.h>
#include <Wt/WGlobal.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WLink.h>

#include <iostream>
#include <math.h>
#include <memory>

namespace view {

TopWidget::TopWidget( graph::GraphSP graph, OptionsSP options ) :
  WContainerWidget(),
  width_( 1000 ),
  height_( 800 )
{
  setLayoutSizeAware( true );

  Wt::WHBoxLayout * const hbox =
    setLayout( Wt::cpp14::make_unique< Wt::WHBoxLayout >() );

  LeftViewWidget * const left_item =
    hbox->addWidget( Wt::cpp14::make_unique< LeftViewWidget >( graph, options ), 1 );
  graph_widget_ = left_item->graphWidget();

  Wt::WContainerWidget * const right_container =
    hbox->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), 2 );
  right_container->setMinimumSize( 500, 1000 );

  Wt::WBorderLayout * const right_layout =
    right_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
  right_layout->setSpacing( 0 );

  right_layout->addWidget( Wt::cpp14::make_unique< RightViewWidget >( graph, graph_widget_, std::move( options ) ),
    Wt::LayoutPosition::Center );

}

TopWidget::~TopWidget(){}

}//namespace view
