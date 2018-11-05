#include <view/LoadWidget.hh>

#include <graph/Graph.hh>
#include <view/GraphWidget.hh>
#include <view/OptionsWidget.hh>


//#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WFileUpload.h>
#include <Wt/WProgressBar.h>
#include <Wt/WBreak.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

namespace view {

namespace {

std::vector< std::string > get_file_lines( std::string const & filename ){
  std::vector< std::string > myLines;
  std::ifstream myFile( filename );
  if( myFile.good() ){
    for( std::string line; std::getline( myFile, line ); ) {
      myLines.push_back(line);
    }
  }
  return myLines;
}

std::string load_file(
  std::vector< std::string > const & file_lines,
  graph::Graph & graph,
  Options & options
){
  //First, options
  int current_line = 0;
  if( file_lines[ current_line ] != "START_OPTIONS" ) {
    return "Error, expected first line to say \"START_OPTIONS\"";
  }
  current_line = options.load( file_lines, current_line ) + 1;

  //Graph
  if( file_lines[ current_line ] != "START_GRAPH" ) {
    return "Error, expected first line to say \"START_OPTIONS\"";
  }
  current_line = graph.loadSelfNodesAndEdges( file_lines, current_line ) + 1;

  return "load successful";
}

}

std::string load_file(
  std::string const & filename,
  graph::Graph & graph,
  Options & options
){
  return load_file( get_file_lines( filename ), graph, * options_ );
}


LoadWidget::LoadWidget(
  RightViewWidget * parent,
  graph::GraphSP const & graph,
  GraphWidget * graph_widget,
  OptionsSP options
) :
  WContainerWidget(),
  OptionsHolder( std::move( options ) ),
  parent_( parent )
{
  //largely copied from https://github.com/emweb/wt/blob/29ae91638e197013f67e7c826317529615d10749/examples/widgetgallery/examples/FileUpload.cpp

  //todo look into WFileDropWidget
  Wt::WFileUpload * fu = addWidget( Wt::cpp14::make_unique< Wt::WFileUpload >() );
  fu->setFileTextSize( 1000 );//Set the maximum file size to 1000 kB.

  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  Wt::WPushButton * upload_button = addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Load" ) );
  Wt::WText * out = addWidget( Wt::cpp14::make_unique< Wt::WText >() );

  upload_button->clicked().connect(
    [=] {
      fu->upload();
      upload_button->disable();
      out->setText("File upload has begun.");
    }
  );

  fu->uploaded().connect(
    [=] {
      upload_button->enable();

      auto const filename = fu->spoolFileName();
      if( filename.size() > 1 ){
	std::vector< std::string > lines = get_file_lines( filename );
	if( lines.size() > 1 ){
	  //TODO clear maps in graph view
	  out->setText( load_file( lines, * graph ) );
	  graph_widget->update();
	  parent_->options_widget()->update();
	}
      }
    }
  );

  fu->fileTooLarge().connect(
    [=] {
      upload_button->enable();
      out->setText("File is too large.");
    }
  );
}

LoadWidget::~LoadWidget(){}

}//namespace view
