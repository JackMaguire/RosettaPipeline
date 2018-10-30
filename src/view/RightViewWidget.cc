#include <view/RightViewWidget.hh>

#include <view/NodeWidget.hh>
#include <view/EdgeWidget.hh>
#include <view/SaveWidget.hh>
#include <view/LoadWidget.hh>
#include <view/OptionsWidget.hh>
#include <view/CompileWidget.hh>

#include <global_data/options.hh>

#include <graph/Graph.hh>
#include <graph/Node.fwd.hh>
#include <graph/Edge.fwd.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>

#include <iostream>
#include <memory>

namespace view {

RightViewWidget::RightViewWidget(
  graph::GraphSP graph,
  GraphWidget * graph_widget
) :
  WTabWidget(),
  graph_( std::move( graph ) ),
  graph_widget_( graph_widget ),
  width_( 500 ),
  height_( 800 )
{
  setLayoutSizeAware( true );

  graph_->registerNewChangeListener( this );

  graph::NodeSP selected_node = graph_->selectedNode();
  if( selected_node ){
    addTab( Wt::cpp14::make_unique< NodeWidget >( selected_node, graph_widget ), "Edit", Wt::ContentLoading::Eager );
  } else {
    graph::EdgeSP selected_edge = graph_->selectedEdge();
    assert( selected_edge );
    addTab( Wt::cpp14::make_unique< EdgeWidget >( selected_edge ), "Edit", Wt::ContentLoading::Eager );
  }

  addTab( Wt::cpp14::make_unique< OptionsWidget >( graph_widget ), "Options", Wt::ContentLoading::Eager );
  addTab( Wt::cpp14::make_unique< SaveWidget >( graph_ ), "Save", Wt::ContentLoading::Eager );
  addTab( Wt::cpp14::make_unique< LoadWidget >( graph_, graph_widget ), "Load", Wt::ContentLoading::Eager );
  addTab( Wt::cpp14::make_unique< CompileWidget >( graph_ ), "Compile", Wt::ContentLoading::Eager );

  setStyleClass("tabwidget");
  setCurrentIndex( 0 );
}

RightViewWidget::~RightViewWidget(){}

void
RightViewWidget::noteChangeInSelection(){

  auto const starting_index = currentIndex();
  removeTab( widget( 0 ) );

  graph::NodeSP selected_node = graph_->selectedNode();
  if( selected_node ){
    insertTab( 0, Wt::cpp14::make_unique< NodeWidget >( selected_node, graph_widget_ ), "Edit", Wt::ContentLoading::Eager );
  } else {
    graph::EdgeSP selected_edge = graph_->selectedEdge();
    assert( selected_edge );
    insertTab( 0, Wt::cpp14::make_unique< EdgeWidget >( selected_edge ), "Edit", Wt::ContentLoading::Eager );
  }

  setCurrentIndex( starting_index );
}

}//namespace view
