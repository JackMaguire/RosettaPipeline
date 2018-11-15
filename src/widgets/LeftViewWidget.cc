#include <widgets/LeftViewWidget.hh>

#include <widgets/GraphWidget.hh>
#include <widgets/GraphToolbarWidget.hh>

#include <graph/Graph.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WLink.h>
#include <Wt/WImage.h>

#include <iostream>
#include <memory>

namespace widgets {

LeftViewWidget::LeftViewWidget(
  graph::GraphSP graph,
  OptionsSP options
) :
  WContainerWidget()
{
  Wt::WVBoxLayout * const vbox =
    setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );

  //LOGO
  Wt::WImage * const image =
    vbox->addWidget( Wt::cpp14::make_unique< Wt::WImage >( Wt::WLink( "media/RosettaPipeline.png" ) ) );
  image->setMinimumSize( 290, 80 );//290 × 80
  image->setMaximumSize( 290, 80 );//290 × 80

  //Edit Area
  Wt::WContainerWidget * const outer_edit_container =
    vbox->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WBorderLayout * const outer_edit_layout =
    outer_edit_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
  Wt::WContainerWidget * const inner_edit_container =
    outer_edit_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), Wt::LayoutPosition::East );
  Wt::WHBoxLayout * const edit_layout =
    inner_edit_container->setLayout( Wt::cpp14::make_unique< Wt::WHBoxLayout >() );
  GraphToolbarWidget * const toolbar =
    edit_layout->addWidget( Wt::cpp14::make_unique< GraphToolbarWidget >() );
  graph_widget_ =
    edit_layout->addWidget( Wt::cpp14::make_unique< GraphWidget >( graph, toolbar ) );
  graph_widget_->setOptions( options );

  //Wt::WContainerWidget * const dummy_container =
  outer_edit_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), Wt::LayoutPosition::Center );

}

LeftViewWidget::~LeftViewWidget(){}

}//namespace widgets
