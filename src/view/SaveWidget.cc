#include <view/SaveWidget.hh>

#include <global_data/options.hh>
#include <graph/Graph.hh>

//#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
//#include <Wt/WFileUpload.h>
//#include <Wt/WProgressBar.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace view {

namespace {

/*std::vector< std::string > get_file_lines( std::string const & filename ){
  std::vector< std::string > myLines;
  std::ifstream myFile( filename );
  if( myFile.good() ){
    for( std::string line; std::getline( myFile, line ); ) {
      myLines.push_back(line);
    }
  }
  return myLines;
}*/

/*std::string save_file(
  std::vector< std::string > const & file_lines,
  graph::GraphSP const & graph
){
  //First, options
  int current_line = 0;
  if( file_lines[ current_line ] != "START_OPTIONS" ) {
    return "Error, expected first line to say \"START_OPTIONS\"";
  }
  current_line = global_data::Options::load( file_lines, current_line ) + 1;

  //Graph
  if( file_lines[ current_line ] != "START_GRAPH" ) {
    return "Error, expected first line to say \"START_OPTIONS\"";
  }
  current_line = graph->loadSelfNodesAndEdges( file_lines, current_line ) + 1;

  return "load successful";
}*/

}

SaveWidget::SaveWidget(
  graph::GraphSP const & graph
) :
  WContainerWidget()
{
  Wt::WLineEdit * line_edit = addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >( "MyProtocol.rpf" ) );

  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  Wt::WPushButton * downloadButton = addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Save" ) );

  downloadButton->clicked().connect(
    [=] {
      //out->setText("File upload has begun.");
    }
  );
}

SaveWidget::~SaveWidget(){}

}//namespace view
