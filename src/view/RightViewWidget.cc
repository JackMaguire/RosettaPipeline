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

namespace {

class SaveAndLoadWidget : public Wt::WContainerWidget {
public:

  SaveAndLoadWidget(
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
      addWidget( Wt::cpp14::make_unique< LoadWidget >( graph, graph_widget ) );
  }

  ~SaveAndLoadWidget() = default;

private:
  SaveWidget * save_widget_;
  LoadWidget * load_widget_;

};

}

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

  addTab( Wt::cpp14::make_unique< WelcomeWidget >(), "Welcome", Wt::ContentLoading::Eager );
  addTab( Wt::cpp14::make_unique< ExamplesWidget >( graph_, graph_widget_ ), "Examples", Wt::ContentLoading::Eager );

  graph::NodeSP selected_node = graph_->selectedNode();
  NodeWidget * node_ptr = 0;
  if( selected_node ){
    auto uniq_node_ptr = Wt::cpp14::make_unique< NodeWidget >( selected_node, graph_widget_ );
    node_ptr = &(*uniq_node_ptr);
    addTab( std::move(uniq_node_ptr), "Edit", Wt::ContentLoading::Eager );
  } else {
    graph::EdgeSP selected_edge = graph_->selectedEdge();
    assert( selected_edge );
    addTab( Wt::cpp14::make_unique< EdgeWidget >( selected_edge ), "Edit", Wt::ContentLoading::Eager );
  }

  auto uniq_options_ptr = Wt::cpp14::make_unique< OptionsWidget >( graph_widget_ );
  options_widget_ = &( *uniq_options_ptr );
  addTab( std::move(uniq_options_ptr), "Options", Wt::ContentLoading::Eager );
  options_widget_->setNodeWidget( node_ptr );

  addTab( Wt::cpp14::make_unique< SaveAndLoadWidget >( graph_, graph_widget_ ), "Save/Load", Wt::ContentLoading::Eager );
  addTab( Wt::cpp14::make_unique< CompileWidget >( graph_ ), "Compile", Wt::ContentLoading::Eager );

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
    NodeWidget * const node_ptr = &(*uniq_node_ptr);
    insertTab( 2, std::move(uniq_node_ptr), "Edit", Wt::ContentLoading::Eager );
    options_widget_->setNodeWidget( node_ptr );
  } else {
    graph::EdgeSP selected_edge = graph_->selectedEdge();
    assert( selected_edge );
    insertTab( 2, Wt::cpp14::make_unique< EdgeWidget >( selected_edge ), "Edit", Wt::ContentLoading::Eager );
    options_widget_->setNodeWidget( 0 );
  }

  setCurrentIndex( starting_index );
}

}//namespace view
