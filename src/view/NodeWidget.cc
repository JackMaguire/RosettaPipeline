#include <view/NodeWidget.hh>

#include <global_data/options.hh>
#include <graph/Node.hh>
#include <view/GraphWidget.hh>

#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>

namespace view {

NodeWidget::NodeWidget(
  graph::NodeSP node,
  GraphWidget * graph_widget
) :
  WContainerWidget(),
  node_( std::move( node ) )
{
  contruct_segment1( graph_widget );
}

NodeWidget::~NodeWidget(){
}

void
NodeWidget::contruct_segment1( GraphWidget * graph_widget ){
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "Title: " ) );

  Wt::WLineEdit * title_edit =
    addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >() );

  title_edit->keyPressed().connect(
    [=] ( Wt::WKeyEvent const & e ) {
      node_->setTitle( e.text().narrow() );
      graph_widget->update();
    }
  );

  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
}

}//namespace view
