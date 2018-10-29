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
  auto bottom_container = layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  construct_segment1( graph_widget, * top_container );

  //bottom_container->resize( Wt::WLength::Auto, 600 );
  auto bottom_layout =
    bottom_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  auto script_editor =
    bottom_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextEdit >( node_->xmlScript() ), Wt::LayoutPosition::Center );
  script_editor->setExtraPlugins( "advcode" );
}

NodeWidget::~NodeWidget(){
}

void
NodeWidget::construct_segment1(
  GraphWidget * graph_widget,
  Wt::WContainerWidget & container
){
  container.addWidget( Wt::cpp14::make_unique< Wt::WText >( "Title: " ) );

  Wt::WLineEdit * title_edit =
    container.addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >( node_->title() ) );

  title_edit->keyPressed().connect(
    [=] ( Wt::WKeyEvent const & e ) {
      node_->setTitle( title_edit->text().narrow() );
      graph_widget->update();
    }
  );

  container.addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  container.addWidget( Wt::cpp14::make_unique< Wt::WText >( "Command: " ) );

  Wt::WLineEdit * command_edit =
    container.addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >( node_->command() ) );

  command_edit->keyPressed().connect(
    [=] ( Wt::WKeyEvent const & e ) {
      node_->setCommand( command_edit->text().narrow() );
    }
  );

  container.addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

}

void
NodeWidget::construct_segment2(
  Wt::WContainerWidget & container
){
  container.addWidget( Wt::cpp14::make_unique< Wt::WText >( "Your Rosetta Flags:" ) );
  container.addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  container.addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( node_->userRosettaFlags() ) );
  container.addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
}


}//namespace view
