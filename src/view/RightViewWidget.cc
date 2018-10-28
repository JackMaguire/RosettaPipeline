#include <view/RightViewWidget.hh>

#include <view/EditWidget.hh>
#include <view/SaveWidget.hh>
#include <view/LoadWidget.hh>
#include <view/CompileWidget.hh>

#include <global_data/options.hh>

#include <graph/Graph.fwd.hh>
//#include <view/graph_view.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>

#include <iostream>
#include <memory>

namespace view {

RightViewWidget::RightViewWidget(
  graph::GraphSP const & graph,
  GraphWidget * graph_widget
) :
  WTabWidget(),
  width_( 500 ),
  height_( 800 )
{
  setLayoutSizeAware( true );
  resize( width_, height_ ); // Provide a default size.

  addTab( Wt::cpp14::make_unique< EditWidget >( graph ), "Edit", Wt::ContentLoading::Eager );
  addTab( Wt::cpp14::make_unique< SaveWidget >( graph ), "Save", Wt::ContentLoading::Eager );
  addTab( Wt::cpp14::make_unique< LoadWidget >( graph, graph_widget ), "Load", Wt::ContentLoading::Eager );
  addTab( Wt::cpp14::make_unique< CompileWidget >( graph ), "Compile", Wt::ContentLoading::Eager );

  setStyleClass("tabwidget");
}

RightViewWidget::~RightViewWidget(){}

}//namespace view
