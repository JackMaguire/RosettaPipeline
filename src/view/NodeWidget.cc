#include <view/NodeWidget.hh>

#include <global_data/options.hh>
#include <graph/Node.hh>
#include <view/GraphWidget.hh>

#include <Wt/WBorderLayout.h>
#include <Wt/WBreak.h>
#include <Wt/WCheckBox.h>
#include <Wt/WGlobal.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WLineEdit.h>
//#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WTextEdit.h>
#include <Wt/WVBoxLayout.h>

namespace view {

NodeWidget::NodeWidget(
  graph::NodeSP node,
  GraphWidget * graph_widget
) :
  WContainerWidget(),
  node_( std::move( node ) )
{
  auto layout = setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );
  layout->setSpacing( 1 );
  auto top_container = layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  auto top_layout =
    top_container->setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );
  auto bottom_container = layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  construct_segment1( graph_widget, * top_layout );
  construct_segment2( * top_layout );
  construct_segment3( * top_layout );
  construct_segment4( * top_layout );

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

  /*command_edit->changed().connect(
    [=] {
      std::cout << "Changed " << command_edit->text() << std::endl;
      node_->setCommand( command_edit->text().narrow() );
    }
  );

  command_edit->validated().connect(
    [=] {
      std::cout << "Validated " << command_edit->text() << std::endl;
      node_->setCommand( command_edit->text().narrow() );
    }
  );*/

  command_edit->textInput().connect(
    [=] {
      std::cout << "textInput " << command_edit->text() << std::endl;
      std::cout << "textInput " << command_edit->text().narrow() << std::endl;
      std::cout << "textInput " << command_edit->text().toUTF8() << std::endl;
      node_->setCommand( command_edit->text().narrow() );
    }
  );

  Wt::WCheckBox * use_default_command_box =
    container->addWidget( Wt::cpp14::make_unique< Wt::WCheckBox >( "Use Default Command" ) );

  use_default_command_box->setChecked( node_->useDefaultCommand() );
  command_edit->setText( node_->getEffectiveCommand() );
  if( node_->useDefaultCommand() ){
    command_edit->setReadOnly( true );
  } else {
    command_edit->setReadOnly( false );
  }
  //command_edit->setHidden(node_->useDefaultCommand() );

  use_default_command_box->checked().connect (
    [=] {
      node_->setUseDefaultCommand( true );
      command_edit->setText( node_->getEffectiveCommand() );
      command_edit->setReadOnly( true );
      //command_edit->setHidden( true );
    }
  );

  use_default_command_box->unChecked().connect (
    [=] {
      node_->setUseDefaultCommand( false );
      command_edit->setText( node_->getEffectiveCommand() );
      command_edit->setReadOnly( false );
      //command_edit->setHidden( false );
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

  flag_area->changed().connect(
    [=] {
      node_->setUserRosettaFlags( flag_area->text().narrow() );
    }
  );

  flag_area->setMinimumSize( 100, 100 );
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
  left_text_area->setReadOnly( true );


  right_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Commonly Used Flags:" ),
    Wt::LayoutPosition::North );
  auto right_text_area =
    right_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( graph::Node::commonFlags() ),
      Wt::LayoutPosition::Center );
  right_text_area->setReadOnly( true );

}

void
NodeWidget::construct_segment4 (
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



  left_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Help:" ),
    Wt::LayoutPosition::North );

  std::string help_message =
    "Click on a node to select a node\n"
    "Click on the arrow of an edge to select that edge\n"
    "Shift+Click on a node/edge to delete it\n"
    "Alt+Click to create a node\n"
    "Alt+Click on a node and drag to another node to create an edge\n";

  auto left_text_area =
    left_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( help_message ),
      Wt::LayoutPosition::Center );
  left_text_area->setReadOnly( true );


  right_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Notes:" ),
    Wt::LayoutPosition::North );
  auto right_text_area =
    right_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( node_->notes() ),
      Wt::LayoutPosition::Center );

  right_text_area->changed().connect(
    [=] {
      node_->setNotes( right_text_area->text().toUTF8() );
    }
  );
}


}//namespace view
