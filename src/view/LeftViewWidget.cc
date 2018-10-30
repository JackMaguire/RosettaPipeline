#include <view/LeftViewWidget.hh>

#include <view/GraphWidget.hh>
#include <global_data/options.hh>
#include <graph/Graph.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WBorderLayout.h>

#include <iostream>
#include <memory>

namespace view {

LeftViewWidget::LeftViewWidget(
  graph::GraphSP graph
) :
  WContainerWidget()
{
  Wt::WVBoxLayout * const vbox =
    setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );

  //LOGO
  Wt::WImage * const image =
    vbox->addWidget( Wt::cpp14::make_unique<Wt::WImage>( Wt::WLink( "media/RosettaPipeline.png" ) ) );
  image->setMinimumSize( 605, 167 );//605 × 167
  image->setMaximumSize( 605, 167 );//605 × 167

  graph_widget_ =
    vbox->addWidget( Wt::cpp14::make_unique< GraphWidget >( graph ) );
  GraphToolbarWidget * const toolbar =
    vbox->addWidget( Wt::cpp14::make_unique< GraphToolbarWidget >( ) );

}

LeftViewWidget::~LeftViewWidget(){}

}//namespace view
