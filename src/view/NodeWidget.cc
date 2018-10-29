#include <view/NodeWidget.hh>

#include <global_data/options.hh>
#include <graph/Node.hh>
#include <view/GraphWidget.hh>

#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WTextEdit.h>

namespace view {

NodeWidget::NodeWidget(
  graph::NodeSP node,
  GraphWidget * graph_widget
) :
  WContainerWidget(),
  node_( std::move( node ) )
{
  auto layout = setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );
  auto top_container = layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  auto top_layout =
    top_container->setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );
  auto bottom_container = layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  construct_segment1( graph_widget, * top_layout );
  construct_segment2( * top_layout );
  construct_segment3( * top_layout );

  auto bottom_layout =
    bottom_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  auto script_editor =
    bottom_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextEdit >( node_->xmlScript() ), Wt::LayoutPosition::Center );
  script_editor->setMinimumSize( 500, 200 );
  script_editor->setExtraPlugins( "advcode" );
}

NodeWidget::~NodeWidget(){
}

void
NodeWidget::construct_segment1(
  GraphWidget * graph_widget,
  Wt::WVBoxLayout & layout
){
  Wt::WContainerWidget * container =
    layout.addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  container->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Title: " ) );

  Wt::WLineEdit * title_edit =
    container->addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >( node_->title() ) );

  title_edit->keyPressed().connect(
    [=] ( Wt::WKeyEvent const & e ) {
      node_->setTitle( title_edit->text().narrow() );
      graph_widget->update();
    }
  );

  container->addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  container->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Command: " ) );

  Wt::WLineEdit * command_edit =
    container->addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >( node_->command() ) );

  command_edit->keyPressed().connect(
    [=] ( Wt::WKeyEvent const & e ) {
      node_->setCommand( command_edit->text().narrow() );
    }
  );

  container->addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

}

void
NodeWidget::construct_segment2(
  Wt::WVBoxLayout & outer_layout
){
  auto container = outer_layout.addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  auto layout =
    container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Your Rosetta Flags:" ),
    Wt::LayoutPosition::North );
  auto flag_area =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( node_->userRosettaFlags() ),
      Wt::LayoutPosition::Center);
}

void
NodeWidget::construct_segment3(
  Wt::WVBoxLayout & outer_layout
){
  auto container =
    outer_layout.addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  auto layout =
    container->setLayout( Wt::cpp14::make_unique< Wt::WHBoxLayout >() );

  auto left_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  auto right_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  auto left_layout =
    left_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
  auto right_layout =
    right_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );



  left_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Auto Rosetta Flags (Generated By Us):" ),
    Wt::LayoutPosition::North );
  auto left_text_area =
    left_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( node_->determineAutoFlags() ),
      Wt::LayoutPosition::Center );


  right_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Commonly Used Flags:" ),
    Wt::LayoutPosition::North );
  auto right_text_area =
    right_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( graph::Node::commonFlags() ),
      Wt::LayoutPosition::Center );

}


}//namespace view
