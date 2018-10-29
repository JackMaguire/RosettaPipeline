#include <view/EditWidget.hh>

#include <view/NodeWidget.hh>
#include <view/EdgeWidget.hh>

#include <graph/Graph.hh>
#include <graph/Node.fwd.hh>
#include <graph/Edge.fwd.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>

#include <iostream>
#include <memory>

namespace view {

EditWidget::EditWidget(
  graph::GraphSP graph
) :
  WContainerWidget(),
  graph_( std::move( graph ) ),
  width_( 500 ),
  height_( 800 )
{
  setLayoutSizeAware( true );

  graph_->registerNewChangeListener( this );

  layout_ = setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  graph::NodeSP selected_node = graph_->selectedNode();
  if( selected_node ){
    current_ = layout_->addWidget( Wt::cpp14::make_unique< NodeWidget >( selected_node ), Wt::LayoutPosition::Center );
  } else {
    graph::EdgeSP selected_edge = graph_->selectedEdge();
    assert( selected_edge );
    current_ = layout_->addWidget( Wt::cpp14::make_unique< EdgeWidget >( selected_edge ), Wt::LayoutPosition::Center );
  }
}

EditWidget::~EditWidget(){}

void
EditWidget::noteChangeInSelection(){
  auto uniq_ptr = layout_->removeWidget( current_ );
  uniq_ptr = 0;

  /*graph::NodeSP selected_node = graph_->selectedNode();
  if( selected_node ){
    current_ = layout_->addWidget( Wt::cpp14::make_unique< NodeWidget >( selected_node ), Wt::LayoutPosition::Center );
  } else {
    graph::EdgeSP selected_edge = graph_->selectedEdge();
    assert( selected_edge );
    current_ = layout_->addWidget( Wt::cpp14::make_unique< EdgeWidget >( selected_edge ), Wt::LayoutPosition::Center );
  }*/

}


}//namespace view
