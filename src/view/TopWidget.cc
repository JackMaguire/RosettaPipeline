#include <view/TopWidget.hh>
#include <view/RightViewWidget.hh>
#include <view/LeftViewWidget.hh>
#include <view/GraphWidget.hh>

//#include <global_data/options.hh>

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

TopWidget::TopWidget( graph::GraphSP const & graph ) :
  WContainerWidget(),
  width_( 1000 ),
  height_( 800 )
{
  setLayoutSizeAware( true );

  Wt::WHBoxLayout * const hbox =
    setLayout( Wt::cpp14::make_unique< Wt::WHBoxLayout >() );

  Wt::WContainerWidget * const left_cont =
    hbox->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), 1 );
  Wt::WVBoxLayout * const left_vbox =
    left_cont->setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );
  Wt::WImage * const image =
    left_vbox->addWidget( Wt::cpp14::make_unique<Wt::WImage>( Wt::WLink( "media/RosettaPipeline.png" ) ) );
  image->setMinimumSize( 605, 167 );//605 × 167
  image->setMaximumSize( 605, 167 );//605 × 167
  GraphWidget * const left_item =
    left_vbox->addWidget( Wt::cpp14::make_unique< GraphWidget >( graph ) );

  Wt::WContainerWidget * const right_container =
    hbox->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), 2 );
  right_container->setMinimumSize( 500, 1000 );

  Wt::WBorderLayout * const right_layout =
    right_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
  right_layout->setSpacing( 0 );
  //right_layout->

  right_layout->addWidget( Wt::cpp14::make_unique< RightViewWidget >( graph, left_item ),
    Wt::LayoutPosition::Center );

}

TopWidget::~TopWidget(){}

}//namespace view
