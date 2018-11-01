#include <view/ExamplesWidget.hh>
#include <view/LoadWidget.hh>

#include <view/GraphWidget.hh>

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

namespace view {

ExamplesWidget::ExamplesWidget(
  graph::GraphSP graph,
  GraphWidget * graph_widget
) :
  WContainerWidget()
{
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "<b>Batch Relax</b>" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "Let's talk about Batch Relax." ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  Wt::WPushButton * batch_relax_upload_button =
    addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Load Batch Relax Example" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  batch_relax_upload_button->clicked().connect(
    [=] {
      std::string const filename = util::getPathToTopDirectory() + "/examples/BatchRelax.txt";
      std::cout << filename << std::endl;
      std::string const load_result = load_file( filename, * graph );
      std::cout << load_result << std::endl;
      if( load_result != "load successful" ) {
	graph_widget->update();
      }
    }
  );

  
}

ExamplesWidget::~ExamplesWidget(){}

}//namespace view
