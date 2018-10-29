#include <view/NodeWidget.hh>

#include <global_data/options.hh>
#include <graph/Node.hh>

#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>

namespace view {

NodeWidget::NodeWidget( graph::NodeSP node ) :
  WContainerWidget(),
  node_( std::move( node ) )
{
  addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( node_->title() ) );
}

NodeWidget::~NodeWidget(){
}

}//namespace view
