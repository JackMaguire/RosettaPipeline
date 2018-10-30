#include <view/OptionsWidget.hh>

#include <global_data/options.hh>
#include <view/GraphWidget.hh>

#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>
#include <Wt/WBreak.h>
#include <Wt/WCheckBox.h>
#include <Wt/WSpinBox.h>

//#include <misc/type_deduction.hh>

#include <iostream>
#include <memory>
#include <limits>

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

  Wt::WCheckBox * delete_unused_intermediate_poses_box =
    addWidget( Wt::cpp14::make_unique< Wt::WCheckBox >( "<b>Delete Unused Intermediate Poses.</b>" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( " This option can save disk space by deleting poses that are no longer needed. We will not delete any poses that result from terminal stages becase we assume you might want to look at those. We will also not delete any input structures." ) )->setWordWrap( true );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  Wt::WCheckBox * serialize_intermediate_poses_box =
    addWidget( Wt::cpp14::make_unique< Wt::WCheckBox >( "<b>Serialize Intermediate Poses.</b>" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( " This option can speed up runs by saving poses in a faster-to-read format. This format cannot be read by any software other than Rosetta, so we will not apply this option to terminal stages (stages with no nodes after). Requires Rosetta to be built with extras=serialization." ) )->setWordWrap( true );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  Wt::WSpinBox * num_proc_spin_box =
    addWidget( Wt::cpp14::make_unique< Wt::WSpinBox >() );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "   <b>Number of processors.</b> This value is saved as $nproc in the run.sh script" ) )->setTextFormat( Wt::TextFormat::XHTML );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  addWidget( Wt::cpp14::make_unique< Wt::WText >( "   <b>Default Run Command.</b> This is the command that is executed for each node unless otherwise specified." ) )->setTextFormat( Wt::TextFormat::XHTML );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  Wt::WLineEdit * command_edit =
    addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >( Options::default_run_command ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  command_edit->setMinimumSize( 400, command_edit->height() );


  //delete_unused_intermediate_poses_box
  delete_unused_intermediate_poses_box->setChecked( Options::delete_unused_intermediate_poses );
  delete_unused_intermediate_poses_box->setTextFormat( Wt::TextFormat::XHTML );
  delete_unused_intermediate_poses_box->checked().connect (
    [=] {
      Options::delete_unused_intermediate_poses = true;
    }
  );
  delete_unused_intermediate_poses_box->unChecked().connect (
    [=] {
      Options::delete_unused_intermediate_poses = false;
    }
  );


  //serialize_intermediate_poses_box
  serialize_intermediate_poses_box->setChecked( Options::serialize_intermediate_poses );
  serialize_intermediate_poses_box->setTextFormat( Wt::TextFormat::XHTML );
  serialize_intermediate_poses_box->checked().connect (
    [=] {
      Options::serialize_intermediate_poses = true;
    }
  );
  serialize_intermediate_poses_box->unChecked().connect (
    [=] {
      Options::serialize_intermediate_poses = false;
    }
  );


  //num_proc_spin_box
  num_proc_spin_box->setValue( Options::num_processors );
  num_proc_spin_box->setMinimum( 1 );
  //int const max = std::numeric_limits< misc::first_agument_t< decltype( num_proc_spin_box::setMaximum ) > >::max();
  int const max = std::numeric_limits< int >::max();
  num_proc_spin_box->setMaximum( max );
  num_proc_spin_box->changed().connect(
    [=] {
      if( num_proc_spin_box->validate() == Wt::ValidationState::Valid ){
	Options::num_processors = num_proc_spin_box->value();
      }
    }
  );


  //command_edit
  command_edit->textInput().connect(
    [=] {
      Options::default_run_command = command_edit->text().toUTF8();
    }
  );

}

}//namespace view
