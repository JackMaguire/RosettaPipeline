#include <widgets/ExamplesWidget.hh>

#include <widgets/OptionsWidget.hh>
#include <widgets/GraphWidget.hh>

#include <serialization.hh>
#include <refresh.hh>

#include <util.hh>

#include <Wt/WLength.h>
#include <Wt/WImage.h>
#include <Wt/WGlobal.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>

#include <iostream>
#include <fstream>
#include <math.h>
#include <memory>

namespace widgets {

ExamplesWidget::ExamplesWidget(
  graph::GraphSP graph,
  OptionsSP options,
  RefreshableElementVecSP refreshers
) :
  WContainerWidget(),
  OptionsHolder( options )
{
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "<b>Batch Relax</b>" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "This workflow breaks FastRelax or FastDesign into stages, filtering out the poorest performing trajecroies at each stage. Notice that each FastRelax tag has repeats=\"1\" instead of the default value of 5, so the total amount of work done for a ingle trajectory is the same as normal." ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  Wt::WPushButton * batch_relax_upload_button =
    addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Load Batch Relax Example" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  //addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  batch_relax_upload_button->clicked().connect(
    [=] {
      std::string const filename = util::getPathToTopDirectory() + "/examples/BatchRelax.rpf";
      std::cout << filename << std::endl;
      std::string const load_result = serialization::load_file( filename, * graph, * options_ );
      std::cout << load_result << std::endl;
      if( load_result != "load successful" ) {
	//TODO?
      }
      refreshers->refresh_all_objects();
      if( action_to_call_upon_load_ ){
	action_to_call_upon_load_();
      }
    }
  );

  
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "<b>Branched Dock-and-Design</b>" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "This non-linear workflow performs a stage of docking and finishes with a stage of FastDesign. It also duplicates the best docking results and runs HBNet on them before feeding them to FastDesign." ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  Wt::WPushButton * dock_design_upload_button =
    addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Load Branched Dock-and-Design Example" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  //addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  dock_design_upload_button->clicked().connect(
    [=] {
      std::string const filename = util::getPathToTopDirectory() + "/examples/DockHBNetDesignTemp.rpf";
      std::cout << filename << std::endl;
      std::string const load_result = serialization::load_file( filename, * graph, * options_ );
      std::cout << load_result << std::endl;
      if( load_result != "load successful" ) {
	//TODO?
      }
      refreshers->refresh_all_objects();
      if( action_to_call_upon_load_ ){
	action_to_call_upon_load_();
      }
    }
  );
  
}

ExamplesWidget::~ExamplesWidget(){}

}//namespace widgets
