#include <widgets/ExamplesWidget.hh>

#include <widgets/OptionsWidget.hh>
#include <widgets/GraphWidget.hh>
#include <serialization.hh>

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
  RightViewWidget * parent,
  graph::GraphSP graph,
  GraphWidget * graph_widget,
  OptionsSP options
) :
  WContainerWidget(),
  OptionsHolder( std::move( options ) ),
  parent_( parent )
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
      std::string const load_result = serialization::load_file( filename, * graph, * options_ );
      std::cout << load_result << std::endl;
      if( load_result != "load successful" ) {
	//TODO
      }
      graph_widget->update();
      parent_->options_widget()->update();
    }
  );

  
}

ExamplesWidget::~ExamplesWidget(){}

}//namespace widgets
