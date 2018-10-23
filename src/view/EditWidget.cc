#include <view/EditWidget.hh>
#include <global_data/options.hh>

#include <graph/Graph.fwd.hh>

#include <Wt/WLength>
#include <Wt/WGlobal>

#include <iostream>
#include <memory>

namespace view {

EditWidget::EditWidget(
  Wt::WContainerWidget * root,
  graph::GraphSP const & graph
) :
  WTabWidget( root ),
  width_( 1000 ),
  height_( 800 )
{
  setLayoutSizeAware( true );
  resize( width_, height_ ); // Provide a default size.

  using namespace Wt;

}

EditWidget::~EditWidget(){}

}//namespace view
