#include <view/RightViewWidget.hh>

#include <view/NodeWidget.hh>
#include <view/EdgeWidget.hh>
#include <view/SaveWidget.hh>
#include <view/LoadWidget.hh>
#include <view/OptionsWidget.hh>
#include <view/CompileWidget.hh>
#include <view/WelcomeWidget.hh>
#include <view/ExamplesWidget.hh>

#include <global_data/options.hh>

#include <graph/Graph.hh>
#include <graph/Node.fwd.hh>
#include <graph/Edge.fwd.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WBreak.h>
#include <Wt/WText.h>

#include <iostream>
#include <memory>

namespace view {

class SaveAndLoadWidget : public Wt::WContainerWidget {
public:

  SaveAndLoadWidget(
    RightViewWidget * parent,
    graph::GraphSP graph,
    GraphWidget * graph_widget
  ) {
    addWidget( Wt::cpp14::make_unique< Wt::WText >( "<b>Save</b>" ) );
    save_widget_ =
      addWidget( Wt::cpp14::make_unique< SaveWidget >( graph ) );
    addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
    addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
    addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
    addWidget( Wt::cpp14::make_unique< Wt::WText >( "<b>Load</b>" ) );
    load_widget_ =
      addWidget( Wt::cpp14::make_unique< LoadWidget >( parent, graph, graph_widget ) );
  }

  ~SaveAndLoadWidget() = default;

private:
  SaveWidget * save_widget_;
  LoadWidget * load_widget_;

};

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

  welcome_widget_ = addTab_tmpl< WelcomeWidget >( "Welcome" );
  examples_widget_ = addTab_tmpl< ExamplesWidget >( "Examples", this, graph_, graph_widget_ );

  graph::NodeSP selected_node = graph_->selectedNode();
  if( selected_node != 0 ){
    node_edit_widget_ = addTab_tmpl< NodeWidget >( "Edit", selected_node, graph_widget_ );
    edge_edit_widget_ = 0;
  } else {
    graph::EdgeSP selected_edge = graph_->selectedEdge();
    assert( selected_edge );
    node_edit_widget_ = 0;
    edge_edit_widget_ = addTab_tmpl< EdgeWidget >( "Edit", selected_edge );
  }

  options_widget_ = addTab_tmpl< OptionsWidget >( "Options", this, graph_widget_ );

  save_and_load_widget_ = addTab_tmpl< SaveAndLoadWidget >( "Save/Load", this, graph_, graph_widget_ );
  compile_widget_ = addTab_tmpl< CompileWidget >( "Compile", graph_ );

  setStyleClass("tabwidget");
  setCurrentIndex( 0 );
}

RightViewWidget::~RightViewWidget(){}

void
RightViewWidget::noteChangeInSelection(){

  auto const starting_index = currentIndex();
  removeTab( widget( 2 ) );

  graph::NodeSP selected_node = graph_->selectedNode();
  if( selected_node ){
    auto uniq_node_ptr = Wt::cpp14::make_unique< NodeWidget >( selected_node, graph_widget_ );
    node_edit_widget_ = &(*uniq_node_ptr);
    insertTab( 2, std::move(uniq_node_ptr), "Edit", Wt::ContentLoading::Eager );

    edge_edit_widget_ = 0;
  } else {
    graph::EdgeSP selected_edge = graph_->selectedEdge();
    assert( selected_edge );

    auto uniq_edge_ptr = Wt::cpp14::make_unique< EdgeWidget >( selected_edge );
    edge_edit_widget_ = &(*uniq_edge_ptr);
    insertTab( 2, std::move(uniq_edge_ptr), "Edit", Wt::ContentLoading::Eager );

    node_edit_widget_ = 0;
  }

  setCurrentIndex( starting_index );
}

}//namespace view
