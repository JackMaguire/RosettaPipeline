#include <widgets/RightViewWidget.hh>

#include <widgets/NodeWidget.hh>
#include <widgets/EdgeWidget.hh>
#include <widgets/SaveWidget.hh>
#include <widgets/LoadWidget.hh>
#include <widgets/OptionsWidget.hh>
#include <widgets/CompileWidget.hh>
#include <widgets/WelcomeWidget.hh>
#include <widgets/ExamplesWidget.hh>
#include <widgets/PublishWidget.hh>

#include <graph/Graph.hh>
#include <graph/Node.fwd.hh>
#include <graph/Edge.fwd.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WBreak.h>
#include <Wt/WText.h>
#include <Wt/WGroupBox.h>
#include <Wt/WPanel.h>

#include <iostream>
#include <memory>

namespace widgets {

SaveAndLoadWidget::SaveAndLoadWidget(
  RightViewWidget * parent,
  graph::GraphSP graph,
  GraphWidget * graph_widget,
  OptionsSP options
)
{
  /*Wt::WPanel * const save_container =
    addWidget( Wt::cpp14::make_unique< Wt::WPanel >() );
  save_container->setTitle( "<b>Save</b>" );
  save_container->setCollapsible( true );
  save_container->addStyleClass( "centered-example" );*/

  Wt::WGroupBox * const publish_container =
    addWidget( Wt::cpp14::make_unique< Wt::WGroupBox >( "Publish" ) );
  //publish_widget_ =
  publish_container->addWidget( Wt::cpp14::make_unique< PublishWidget >( graph, std::move( options ) ) );

  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
 
  Wt::WGroupBox * const save_container =
    addWidget( Wt::cpp14::make_unique< Wt::WGroupBox >( "Save" ) );
  save_widget_ =
    save_container->addWidget( Wt::cpp14::make_unique< SaveWidget >( graph, options ) );

  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  Wt::WGroupBox * const load_container =
    addWidget( Wt::cpp14::make_unique< Wt::WGroupBox >( "Load" ) );
  load_widget_ =
    load_container->addWidget( Wt::cpp14::make_unique< LoadWidget >( parent, graph, graph_widget, options ) );
}

SaveAndLoadWidget::~SaveAndLoadWidget() = default;

void
SaveAndLoadWidget::setOptions( OptionsSP options ){
  save_widget_->setOptions( options );
  load_widget_->setOptions( options );
}


RightViewWidget::RightViewWidget(
  graph::GraphSP graph,
  GraphWidget * graph_widget,
  OptionsSP options
) :
  WTabWidget(),
  OptionsHolder( options ),
  graph_( std::move( graph ) ),
  graph_widget_( graph_widget ),
  width_( 500 ),
  height_( 800 )
{
  setLayoutSizeAware( true );

  graph_->registerNewChangeListener( this );

  welcome_widget_ = addTab_tmpl< WelcomeWidget >( "Welcome" );
  examples_widget_ = addTab_tmpl< ExamplesWidget >( "Examples", this, graph_, graph_widget_, options_ );

  graph::NodeSP selected_node = graph_->selectedNode();
  if( selected_node != 0 ){
    node_edit_widget_ = addTab_tmpl< NodeWidget >( "Edit", selected_node, graph_widget_, options );
    edge_edit_widget_ = 0;
  } else {
    graph::EdgeSP selected_edge = graph_->selectedEdge();
    assert( selected_edge );
    node_edit_widget_ = 0;
    edge_edit_widget_ = addTab_tmpl< EdgeWidget >( "Edit", selected_edge );
  }

  options_widget_ = addTab_tmpl< OptionsWidget >( "Options", this, graph_widget_, options_ );

  save_and_load_widget_ = addTab_tmpl< SaveAndLoadWidget >(
    "Save/Load",
    this, graph_, graph_widget_, options_
  );
  compile_widget_ = addTab_tmpl< CompileWidget >( "Compile", graph_, options_ );

  setStyleClass("tabwidget");
  setCurrentIndex( 0 );

  setOptionsDownward( std::move( options ) );
}

RightViewWidget::~RightViewWidget(){}

void
RightViewWidget::noteChangeInSelection(){

  auto const starting_index = currentIndex();
  removeTab( widget( 2 ) );

  graph::NodeSP selected_node = graph_->selectedNode();
  if( selected_node ){
    auto uniq_node_ptr = Wt::cpp14::make_unique< NodeWidget >( selected_node, graph_widget_, options_ );
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

void
RightViewWidget::setOptionsDownward( OptionsSP options ){
  assert( examples_widget_ );
  examples_widget_->setOptions( options );

  assert( options_widget_ );
  options_widget_->setOptions( options );

  assert( save_and_load_widget_ );
  save_and_load_widget_->setOptions( options );

  assert( compile_widget_ );
  compile_widget_->setOptions( options );

  setOptions( std::move( options ) );
}


}//namespace widgets