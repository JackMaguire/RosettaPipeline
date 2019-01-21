#include <widgets/BashNodeWidget.hh>

#include <widgets/RSBashNodeWidget.hh>
#include <widgets/BashBashNodeWidget.hh>
#include <widgets/GraphWidget.fwd.hh>

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

  Wt::WContainerWidget * const north_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), Wt::LayoutPosition::North );

  Wt::WContainerWidget * const center_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), Wt::LayoutPosition::Center );

  Wt::WStackedWidget * const stack =
    center_container->addWidget( Wt::cpp14::make_unique< Wt::WStackedWidget >() );

  //First is RSNode
  stack->addWidget(
    Wt::cpp14::make_unique< RSBashNodeWidget >( node_, graph_widget, options )
  );

  //Second is BashNode
  stack->addWidget(
    Wt::cpp14::make_unique< BashBashNodeWidget >( node_, graph_widget, options )
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
