#include <view/OptionsWidget.hh>

#include <global_data/options.hh>
#include <view/GraphWidget.hh>

#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>

#include <iostream>
#include <memory>

using namespace global_data;

namespace view {

OptionsWidget::OptionsWidget( GraphWidget * graph_widget ) :
  WContainerWidget()
{
  setup_view_options( graph_widget );

}

OptionsWidget::~OptionsWidget(){}

void
OptionsWidget::setup_view_options( GraphWidget * graph_widget ){
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "View Options:" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  Wt::WPushButton * zoom_out_button =
    addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Zoom Out" ) );
  Wt::WPushButton * zoom_in_button =
    addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Zoom In" ) );

  zoom_out_button->clicked().connect(
    [=](){
      if( Options::grid_size > 1 ) {
	--Options::grid_size;
	graph_widget->update();
      }
    }
  );

  zoom_in_button->clicked().connect(
    [=](){
      ++Options::grid_size;
      graph_widget->update();
    }
  );

}

}//namespace view
