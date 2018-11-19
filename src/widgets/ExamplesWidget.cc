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
  addExample(
    "Batch Relax",
    "This workflow breaks FastRelax or FastDesign into stages, filtering out the poorest performing trajecroies at each stage. Notice that each FastRelax tag has repeats=\"1\" instead of the default value of 5, so the total amount of work done for a ingle trajectory is the same as normal.",
    "BatchRelax.rpf",
    graph, options, refreshers
  );

  addExample(
    "Branched Dock-and-Design",
    "This non-linear workflow performs a stage of docking and finishes with a stage of FastDesign. It also duplicates the best docking results and runs HBNet on them before feeding them to FastDesign.",
    "DockHBNetDesignTemp.rpf",
    graph, options, refreshers
  );
}

ExamplesWidget::~ExamplesWidget(){}

void ExamplesWidget::addExample(
  std::string const & title,
  std::string const & description,
  std::string const & filename,
  graph::GraphSP graph,
  OptionsSP options,
  RefreshableElementVecSP refreshers
){
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "<b>" + title + "</b>" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( description ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  Wt::WPushButton * const upload_button =
    addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Load "+ title +" Example" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  //addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  upload_button->clicked().connect(
    [=] {
      std::string const fname = util::getPathToTopDirectory() + "/examples/" + filename;
      std::string const load_result = serialization::load_file( fname, * graph, * options_ );
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

}//namespace widgets
