#include <view/EdgeWidget.hh>

#include <global_data/options.hh>
#include <graph/Edge.hh>

#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>

namespace view {

EdgeWidget::EdgeWidget( graph::EdgeSP node ) :
  WContainerWidget(),
  node_( std::move( node ) )
{
  addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( node_->title() ) );
}

EdgeWidget::~EdgeWidget(){
}

}//namespace view
