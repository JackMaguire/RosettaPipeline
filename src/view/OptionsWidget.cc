#include <view/OptionsWidget.hh>

#include <global_data/options.hh>
#include <view/GraphWidget.hh>

#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>
#include <Wt/WBreak.h>
#include <Wt/WCheckBox.h>

#include <iostream>
#include <memory>

using namespace global_data;

namespace view {

OptionsWidget::OptionsWidget( GraphWidget * graph_widget ) :
  WContainerWidget()
{
  setup_view_options( graph_widget );
  setup_run_options();
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
  Wt::WCheckBox * show_node_titles_box =
    addWidget( Wt::cpp14::make_unique< Wt::WCheckBox >( "Show Node Titles" ) );
  Wt::WCheckBox * orient_node_titles_box =
    addWidget( Wt::cpp14::make_unique< Wt::WCheckBox >( "Put Node Titles To Side" ) );

  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

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


  show_node_titles_box->setChecked( Options::show_node_titles );

  show_node_titles_box->checked().connect (
    [=] {
      Options::show_node_titles = true;
      graph_widget->update();
    }
  );

  show_node_titles_box->unChecked().connect (
    [=] {
      Options::show_node_titles = false;
      graph_widget->update();
    }
  );


  orient_node_titles_box->setChecked( Options::put_node_titles_to_side );

  orient_node_titles_box->checked().connect (
    [=] {
      Options::put_node_titles_to_side = true;
      graph_widget->update();
    }
  );

  orient_node_titles_box->unChecked().connect (
    [=] {
      Options::put_node_titles_to_side = false;
      graph_widget->update();
    }
  );

}

void
OptionsWidget::setup_run_options(){
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "Run Options:" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

}

}//namespace view
