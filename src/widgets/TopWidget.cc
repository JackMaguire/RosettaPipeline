#include <widgets/TopWidget.hh>
#include <widgets/RightViewWidget.hh>
#include <widgets/LeftViewWidget.hh>
#include <widgets/GraphWidget.hh>
#include <widgets/GraphToolbarWidget.hh>

#include <refresh.hh>

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

namespace widgets {

TopWidget::TopWidget( graph::GraphSP graph, OptionsSP options ) :
  WContainerWidget(),
  width_( 1000 ),
  height_( 800 )
{
  setLayoutSizeAware( true );

  RefreshableElementVecSP refreshable_objects = std::make_shared< RefreshableElementVec >();

  Wt::WHBoxLayout * const hbox =
    setLayout( Wt::cpp14::make_unique< Wt::WHBoxLayout >() );

  LeftViewWidget * const left_item =
    hbox->addWidget( Wt::cpp14::make_unique< LeftViewWidget >( graph, options ), 1 );
  graph_widget_ = left_item->graphWidget();
  assert( graph_widget_ );
  refreshable_objects->push_back( graph_widget_ );

  Wt::WContainerWidget * const right_container =
    hbox->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), 2 );
  right_container->setMinimumSize( 500, 1000 );

  Wt::WBorderLayout * const right_layout =
    right_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
  right_layout->setSpacing( 0 );

  right_layout->addWidget(
    Wt::cpp14::make_unique< RightViewWidget >( graph, graph_widget_, options, refreshable_objects ),
    Wt::LayoutPosition::Center );

}

TopWidget::~TopWidget(){}

}//namespace widgets
