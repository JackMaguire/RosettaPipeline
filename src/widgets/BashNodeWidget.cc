#include <widgets/BashNodeWidget.hh>

#include <graph/Node.hh>
#include <listeners.hh>

#include <Wt/WBorderLayout.h>
#include <Wt/WBreak.h>
#include <Wt/WCheckBox.h>
#include <Wt/WGlobal.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WLineEdit.h>
//#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WTextEdit.h>
#include <Wt/WVBoxLayout.h>

namespace widgets {

namespace {


}

BashNodeWidget::BashNodeWidget(
  graph::NodeSP node,
  GraphWidget * const graph_widget,
  OptionsSP options
) :
  WContainerWidget(),
  OptionsHolder( options ),
  node_( node )
{
  Wt::WBorderLayout * const layout =
    setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  //////////
  // NORTH

  Wt::WContainerWidget * const north_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), Wt::LayoutPosition::North );

  north_container->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Title: " ) );

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

  //////////
  // CENTER

  Wt::WContainerWidget * const center_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), Wt::LayoutPosition::Center );
  Wt::WBorderLayout * const center_layout =
    center_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  Wt::WTextArea * const bash_script_text_area =
    center_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( node_->getBashScript() ), Wt::LayoutPosition::Center );
  bash_script_text_area->setMinimumSize( 500, 500 );

  bash_script_text_area->changed().connect(
    [=] {
      node_->setBashScript( bash_script_text_area->text().toUTF8() );
    }
  );
}

BashNodeWidget::~BashNodeWidget(){
}


void
BashNodeWidget::updateDefaultCommand( std::string const & new_command ) {
  /*if( node_->useDefaultCommand() ){
    command_edit_->setText( new_command );
  }*/
}


}//namespace widgets
