#include <view/OptionsWidget.hh>

#include <global_data/options.hh>
#include <view/GraphWidget.hh>
#include <view/NodeWidget.hh>

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

OptionsWidget::OptionsWidget(
  RightViewWidget * parent,
  GraphWidget * graph_widget
) :
  WContainerWidget(),
  parent_( parent )
{
  setup_view_options( graph_widget );
  setup_run_options();
  update();
}

OptionsWidget::~OptionsWidget(){}

void
OptionsWidget::setup_view_options( GraphWidget * graph_widget ){
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "View Options:" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  zoom_out_button_ =
    addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Zoom Out" ) );
  zoom_in_button_ =
    addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Zoom In" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "        " ) );
  show_node_titles_box_ =
    addWidget( Wt::cpp14::make_unique< Wt::WCheckBox >( "Show Node Titles" ) );
  orient_node_titles_box_ =
    addWidget( Wt::cpp14::make_unique< Wt::WCheckBox >( "Put Node Titles To Side" ) );

  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  zoom_out_button_->clicked().connect(
    [=](){
      if( Options::grid_size > 1 ) {
	--Options::grid_size;
	graph_widget->update();
      }
    }
  );

  zoom_in_button_->clicked().connect(
    [=](){
      ++Options::grid_size;
      graph_widget->update();
    }
  );


  show_node_titles_box_->checked().connect (
    [=] {
      Options::show_node_titles = true;
      graph_widget->update();
    }
  );

  show_node_titles_box_->unChecked().connect (
    [=] {
      Options::show_node_titles = false;
      graph_widget->update();
    }
  );

  orient_node_titles_box_->checked().connect (
    [=] {
      Options::put_node_titles_to_side = true;
      graph_widget->update();
    }
  );

  orient_node_titles_box_->unChecked().connect (
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

  delete_unused_intermediate_poses_box_ =
    addWidget( Wt::cpp14::make_unique< Wt::WCheckBox >( "<b>Delete Unused Intermediate Poses.</b>" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( " This option can save disk space by deleting poses that are no longer needed. We will not delete any poses that result from terminal stages becase we assume you might want to look at those. We will also not delete any input structures." ) )->setWordWrap( true );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  serialize_intermediate_poses_box_ =
    addWidget( Wt::cpp14::make_unique< Wt::WCheckBox >( "<b>Serialize Intermediate Poses.</b>" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( " This option can speed up runs by saving poses in a faster-to-read format. This format cannot be read by any software other than Rosetta, so we will not apply this option to terminal stages (stages with no nodes after). Requires Rosetta to be built with extras=serialization." ) )->setWordWrap( true );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  num_proc_spin_box_ =
    addWidget( Wt::cpp14::make_unique< Wt::WSpinBox >() );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "   <b>Number of processors.</b> This value is saved as $nproc in the run.sh script" ) )->setTextFormat( Wt::TextFormat::XHTML );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  addWidget( Wt::cpp14::make_unique< Wt::WText >( "   <b>Default Run Command.</b> This is the command that is executed for each node unless otherwise specified." ) )->setTextFormat( Wt::TextFormat::XHTML );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  command_edit_ =
    addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >( Options::default_run_command ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  command_edit_->setMinimumSize( 500, command_edit_->height() );
  command_edit_->setInline( true );
  command_edit_->setFormObject( true );


  //delete_unused_intermediate_poses_box_
  delete_unused_intermediate_poses_box_->setTextFormat( Wt::TextFormat::XHTML );
  delete_unused_intermediate_poses_box_->checked().connect (
    [=] {
      Options::delete_unused_intermediate_poses = true;
    }
  );
  delete_unused_intermediate_poses_box_->unChecked().connect (
    [=] {
      Options::delete_unused_intermediate_poses = false;
    }
  );


  //serialize_intermediate_poses_box_
  serialize_intermediate_poses_box_->setTextFormat( Wt::TextFormat::XHTML );
  serialize_intermediate_poses_box_->checked().connect (
    [=] {
      Options::serialize_intermediate_poses = true;
    }
  );
  serialize_intermediate_poses_box_->unChecked().connect (
    [=] {
      Options::serialize_intermediate_poses = false;
    }
  );


  //num_proc_spin_box_
  num_proc_spin_box_->setMinimum( 1 );
  //int const max = std::numeric_limits< misc::first_agument_t< decltype( num_proc_spin_box_::setMaximum ) > >::max();
  int const max = std::numeric_limits< int >::max();
  num_proc_spin_box_->setMaximum( max );
  num_proc_spin_box_->changed().connect(
    [=] {
      if( num_proc_spin_box_->validate() == Wt::ValidationState::Valid ){
	Options::num_processors = num_proc_spin_box_->value();
      }
    }
  );


  //command_edit_
  command_edit_->textInput().connect(
    [=] {
      Options::default_run_command = command_edit_->text().toUTF8();
      if( parent_->node_edit_widget() != 0 ){
	parent_->node_edit_widget()->updateDefaultCommand( Options::default_run_command );
      }
    }
  );

}

void
OptionsWidget::update(){
  //View
  show_node_titles_box_->setChecked( Options::show_node_titles );
  orient_node_titles_box_->setChecked( Options::put_node_titles_to_side );

  //Run
  command_edit_->setText( Options::default_run_command ) );
  serialize_intermediate_poses_box_->setChecked( Options::serialize_intermediate_poses );
  delete_unused_intermediate_poses_box_->setChecked( Options::delete_unused_intermediate_poses );  
  num_proc_spin_box_->setValue( Options::num_processors );
}

}//namespace view
