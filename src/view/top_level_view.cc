#include <view/top_level_view.hh>
#include <view/RightViewWidget.hh>
#include <global_data/options.hh>

#include <graph/Graph.hh>
#include <graph/Edge.hh>
#include <graph/Node.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>

#include <iostream>
#include <math.h>
#include <memory>

namespace view {

TopWidget::TopWidget(
  graph::GraphSP const & graph
) :
  WContainerWidget()
{
  auto hbox = setLayout( Wt::cpp14::make_unique< Wt::WHBoxLayout >() );
  GraphWidget * left_item =
    hbox->addWidget( Wt::cpp14::make_unique< GraphWidget >( graph ) );
  hbox->addWidget( Wt::cpp14::make_unique< RightViewWidget >( graph, left_item ) );
}

TopWidget::~TopWidget(){}

}//namespace view
