#include <widgets/NodeWidget.hh>

#include <widgets/RSNodeWidget.hh>
#include <widgets/BashNodeWidget.hh>
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
#include <Wt/WMenu.h>
#include <Wt/WVBoxLayout.h>

namespace widgets {

namespace {


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
  Wt::WBorderLayout * const layout =
    setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  Wt::WContainerWidget * const north_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), Wt::LayoutPosition::North );

  Wt::WContainerWidget * const center_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), Wt::LayoutPosition::Center );

  Wt::WStackedWidget * const stack =
    center_container->addWidget( Wt::cpp14::make_unique< Wt::WStackedWidget >() );

  /*
  //First is RSNode
  stack->addWidget(
    Wt::cpp14::make_unique< RSNodeWidget >( node_, graph_widget, options )
  );

  //Second is BashNode
  stack->addWidget(
    Wt::cpp14::make_unique< BashNodeWidget >( node_, graph_widget, options )
  );
 */


  Wt::WMenu * const menu =
    top_container->addWidget( Wt::cpp14::make_unique< Wt::WMenu >( stack.get() ) );
  menu->setStyleClass("nav nav-pills");
  menu->insertItem(
    graph::NodeType::ROSETTA_SCRIPTS,
    "Rosetta Scripts",
    Wt::cpp14::make_unique< RSNodeWidget >( node_, graph_widget, options )
  );
  menu->addItem(
    graph::NodeType::BASH,
    "Bash Script",
    Wt::cpp14::make_unique< BashNodeWidget >( node_, graph_widget, options )
  );

  menu->itemSelected().connect(
    [=](){
      auto const index = menu->currentIndex();
      node->setNodeType( graph::NodeType( index ) );
    }
  );

}

NodeWidget::~NodeWidget(){
}


void
NodeWidget::updateDefaultCommand( std::string const & new_command ) {
  /*if( node_->useDefaultCommand() ){
    command_edit_->setText( new_command );
  }*/
}


}//namespace widgets
