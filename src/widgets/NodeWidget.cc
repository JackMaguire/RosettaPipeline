#include <widgets/NodeWidget.hh>

#include <graph/Node.hh>
#include <widgets/GraphWidget.hh>
#include <listeners.hh>

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

namespace widgets {

namespace {

class ChooseExistingScriptWidget : public Wt::WContainerWidget {
public:
  ChooseExistingScriptWidget(
    graph::NodeSP node
  ) :
    WContainerWidget(),
    node_( node )
    {
      /*Wt::WHBoxLayout * const layout =
	setLayout( Wt::cpp14::make_unique< Wt::WHBoxLayout >() );
      
      Wt::WCheckBox * const box =
	layout->addWidget( Wt::cpp14::make_unique< Wt::WCheckBox >( "Use Existing File" ) );

      Wt::WCheckBox * const */
    }


private:
  graph::NodeSP node_;
};

}

NodeWidget::NodeWidget(
  graph::NodeSP node,
  GraphWidget * const graph_widget,
  OptionsSP options
) :
  WContainerWidget(),
  OptionsHolder( options ),
  node_( node )
{
  Wt::WVBoxLayout * const layout =
    setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );
  layout->setSpacing( 1 );
  Wt::WContainerWidget * const top_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WVBoxLayout * const top_layout =
    top_container->setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );
  Wt::WContainerWidget * const bottom_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  construct_segment1( graph_widget, * top_layout );
  construct_segment2( * top_layout );
  construct_segment3( * top_layout );
  //construct_segment4( * top_layout );

  Wt::WBorderLayout * const bottom_layout =
    bottom_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
  construct_segment5( * bottom_layout );


  //script_editor->setExtraPlugins( "bbcode" );
}

NodeWidget::~NodeWidget(){
}

void
NodeWidget::construct_segment1(
  GraphWidget * const graph_widget,
  Wt::WVBoxLayout & layout
){
  Wt::WContainerWidget * const container =
    layout.addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  container->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Title: " ) );

  Wt::WLineEdit * const title_edit =
    container->addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >( node_->title() ) );
  title_edit->setInline( true );
  title_edit->setFormObject( true );
  title_edit->textInput().connect(
    [=] {
      node_->setTitle( title_edit->text().narrow() );
      graph_widget->update();
    }
  );

  container->addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );



  Wt::WContainerWidget * const cmd_container =
    container->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
    
  Wt::WBorderLayout * const cmd_layout =
    cmd_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  cmd_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Command: " ),
    Wt::LayoutPosition::West );

  command_edit_ =
    cmd_layout->addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >( node_->command() ),
      Wt::LayoutPosition::Center );
  command_edit_->setInline( true );
  command_edit_->setFormObject( true );
  command_edit_->textInput().connect(
    [=] {
      node_->setCommand( command_edit_->text().narrow() );
    }
  );

  Wt::WCheckBox * const use_default_command_box =
    cmd_layout->addWidget( Wt::cpp14::make_unique< Wt::WCheckBox >( "Use Default Command" ),
      Wt::LayoutPosition::East );

  use_default_command_box->setChecked( node_->useDefaultCommand() );
  command_edit_->setText( node_->getEffectiveCommand( * options_ ) );
  if( node_->useDefaultCommand() ){
    command_edit_->setReadOnly( true );
  } else {
    command_edit_->setReadOnly( false );
  }
  //command_edit_->setHidden(node_->useDefaultCommand() );

  use_default_command_box->checked().connect (
    [=] {
      node_->setUseDefaultCommand( true );
      command_edit_->setText( node_->getEffectiveCommand( * options_ ) );
      command_edit_->setReadOnly( true );
      //command_edit_->setHidden( true );
    }
  );

  use_default_command_box->unChecked().connect (
    [=] {
      node_->setUseDefaultCommand( false );
      command_edit_->setText( node_->getEffectiveCommand( * options_ ) );
      command_edit_->setReadOnly( false );
      //command_edit_->setHidden( false );
    }
  );

  container->addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

}

void
NodeWidget::construct_segment2(
  Wt::WVBoxLayout & outer_layout
){
  Wt::WContainerWidget * const container =
    outer_layout.addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WHBoxLayout * const layout =
    container->setLayout( Wt::cpp14::make_unique< Wt::WHBoxLayout >() );

  Wt::WContainerWidget * const left_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WContainerWidget * const right_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  Wt::WBorderLayout * const left_layout =
    left_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
  Wt::WBorderLayout * const right_layout =
    right_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );


  left_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Your Rosetta Flags:" ),
    Wt::LayoutPosition::North );
  Wt::WTextArea * const flag_area =
    left_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( node_->userRosettaFlags() ),
      Wt::LayoutPosition::Center);

  flag_area->changed().connect(
    [=] {
      node_->setUserRosettaFlags( flag_area->text().narrow() );
    }
  );

  flag_area->setMinimumSize( 200, 100 );


  right_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Notes:" ),
    Wt::LayoutPosition::North );
  Wt::WTextArea * const right_text_area =
    right_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( node_->notes() ),
      Wt::LayoutPosition::Center );
  right_text_area->setMinimumSize( 200, 100 );

  right_text_area->changed().connect(
    [=] {
      node_->setNotes( right_text_area->text().toUTF8() );
    }
  );
}


void
NodeWidget::construct_segment3(
  Wt::WVBoxLayout & outer_layout
){
  Wt::WContainerWidget * const container =
    outer_layout.addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WHBoxLayout * const layout =
    container->setLayout( Wt::cpp14::make_unique< Wt::WHBoxLayout >() );

  Wt::WContainerWidget * const left_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WContainerWidget * const right_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  Wt::WBorderLayout * const left_layout =
    left_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
  Wt::WBorderLayout * const right_layout =
    right_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );



  left_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Auto Rosetta Flags (Generated By Us):" ),
    Wt::LayoutPosition::North );
  Wt::WTextArea * const left_text_area =
    left_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( node_->determineAutoFlags() ),
      Wt::LayoutPosition::Center );
  left_text_area->setReadOnly( true );
  left_text_area->setMinimumSize( 200, 100 );

  right_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Commonly Used Flags:" ),
    Wt::LayoutPosition::North );
  Wt::WTextArea * const right_text_area =
    right_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( graph::Node::commonFlags() ),
      Wt::LayoutPosition::Center );
  right_text_area->setReadOnly( true );
  right_text_area->setMinimumSize( 200, 100 );
}

void
NodeWidget::construct_segment4 (
  Wt::WVBoxLayout & outer_layout
){
  Wt::WContainerWidget * const container =
    outer_layout.addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WHBoxLayout * const layout =
    container->setLayout( Wt::cpp14::make_unique< Wt::WHBoxLayout >() );

  Wt::WContainerWidget * const left_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WContainerWidget * const right_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  Wt::WBorderLayout * const left_layout =
    left_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
  Wt::WBorderLayout * const right_layout =
    right_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );



  left_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Help:" ),
    Wt::LayoutPosition::North );

  std::string const help_message =
    "Click on a node to select a node\n"
    "Click on the arrow of an edge to select that edge\n"
    "Shift+Click on a node/edge to delete it\n"
    "Alt+Click to create a node\n"
    "Alt+Click on a node and drag to another node to create an edge\n";

  Wt::WTextArea * const left_text_area =
    left_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( help_message ),
      Wt::LayoutPosition::Center );
  left_text_area->setReadOnly( true );
  left_text_area->setMinimumSize( 200, 100 );

  right_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Notes:" ),
    Wt::LayoutPosition::North );
  Wt::WTextArea * const right_text_area =
    right_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( node_->notes() ),
      Wt::LayoutPosition::Center );
  right_text_area->setMinimumSize( 200, 100 );

  right_text_area->changed().connect(
    [=] {
      node_->setNotes( right_text_area->text().toUTF8() );
    }
  );
}

void
NodeWidget::construct_segment5 (
  Wt::WBorderLayout & outer_layout
){
  Wt::WTextArea * const script_editor =
    outer_layout.addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( node_->xmlScript() ),
      Wt::LayoutPosition::Center );
  script_editor->setMinimumSize( 500, 400 );
  script_editor->setInline( true );
  script_editor->setFormObject( true );

  script_editor->textInput().connect(
    [=] {
      node_->setXMLScript( script_editor->text().narrow() );
    }
  );

}

void
NodeWidget::updateDefaultCommand( std::string const & new_command ) {
  if( node_->useDefaultCommand() ){
    command_edit_->setText( new_command );
  }
}


}//namespace widgets
