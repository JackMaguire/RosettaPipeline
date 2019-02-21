#include <widgets/NodeWidget.hh>

#include <widgets/RSNodeWidget.hh>
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
    north_container->addWidget( Wt::cpp14::make_unique< Wt::WMenu >( stack ) );
  menu->setStyleClass("nav nav-pills");

  auto rs_ptr = Wt::cpp14::make_unique< RSNodeWidget >( node_, graph_widget, options );
  rs_node_widget_ = rs_ptr.get();
  menu->insertItem(
    graph::NodeType::ROSETTA_SCRIPTS,
    "Rosetta Scripts",
    std::move( rs_ptr )
  );
  menu->insertItem(
    graph::NodeType::BASH,
    "Bash Script",
    Wt::cpp14::make_unique< BashNodeWidget >( node_, graph_widget, options )
  );

  if( node->getNodeType() == graph::NodeType::ROSETTA_SCRIPTS ){
    menu->select( 0 );
  } else {
    menu->select( 1 );
  }
  //menu->select( int( node->getNodeType() ) );


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
  if( rs_node_widget_ != 0 ){
    rs_node_widget_->updateDefaultCommand( new_command );
  }
  /*if( node_->useDefaultCommand() ){
    command_edit_->setText( new_command );
  }*/
}


}//namespace widgets
