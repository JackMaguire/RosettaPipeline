#include <view/RightViewWidget.hh>

#include <view/EditWidget.hh>
#include <view/SaveWidget.hh>
#include <view/LoadWidget.hh>
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
  width_( 500 ),
  height_( 800 )
{
  setLayoutSizeAware( true );
  //resize( width_, height_ ); // Provide a default size.

  graph_->registerNewChangeListener( this );

  graph::NodeSP selected_node = graph_->selectedNode();
  if( selected_node ){
    addTab( Wt::cpp14::make_unique< NodeWidget >( selected_node ), "Edit", Wt::ContentLoading::Eager );
  } else {
    graph::EdgeSP selected_edge = graph_->selectedEdge();
    assert( selected_edge );
    addTab( Wt::cpp14::make_unique< EdgeWidget >( selected_edge ), "Edit", Wt::ContentLoading::Eager );
  }

  //addTab( Wt::cpp14::make_unique< EditWidget >( graph_ ), "Edit", Wt::ContentLoading::Eager );
  addTab( Wt::cpp14::make_unique< SaveWidget >( graph_ ), "Save", Wt::ContentLoading::Eager );
  addTab( Wt::cpp14::make_unique< LoadWidget >( graph_, graph_widget ), "Load", Wt::ContentLoading::Eager );
  addTab( Wt::cpp14::make_unique< CompileWidget >( graph_ ), "Compile", Wt::ContentLoading::Eager );

  setStyleClass("tabwidget");
}

RightViewWidget::~RightViewWidget(){}

void
RightViewWidget::noteChangeInSelection(){

  bool const selected_new_tab = currentIndex() == 0;
  removeTab( itemAt( 0 ) );

  graph::NodeSP selected_node = graph_->selectedNode();
  if( selected_node ){
    insertTab( 0, Wt::cpp14::make_unique< NodeWidget >( selected_node ), "Edit", Wt::ContentLoading::Eager );
  } else {
    graph::EdgeSP selected_edge = graph_->selectedEdge();
    assert( selected_edge );
    insertTab( 0, Wt::cpp14::make_unique< EdgeWidget >( selected_edge ), "Edit", Wt::ContentLoading::Eager );
  }

  if( selected_new_tab ) {
    setCurrentIndex( 0 );
  }

}

}//namespace view
