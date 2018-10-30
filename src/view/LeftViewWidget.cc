#include <view/LeftViewWidget.hh>

#include <view/GraphWidget.hh>
#include <global_data/options.hh>
#include <graph/Graph.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>

#include <iostream>
#include <memory>

namespace view {

LeftViewWidget::LeftViewWidget(
  graph::GraphSP graph
) :
  WContainerWidget()
{
  Wt::WBorderLayout * const layout =
    setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );


  graph_widget_ =
    layout->addWidget( Wt::cpp14::make_unique< GraphWidget >( graph ), Wt::LayoutPosition::Center );

}

LeftViewWidget::~LeftViewWidget(){}

}//namespace view
