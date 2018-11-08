#include <serialization.hh>

#include <graph/Graph.hh>

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>


namespace serialization {

////////
//SAVE//
////////

std::string save(
  Options const & options,
  graph::Graph const & graph
) {
  std::vector< std::string > save_lines;
  options.save( save_lines );
  graph.saveSelfNodesAndEdges( save_lines );
    
  std::stringstream ss;
  for( std::string const & line : save_lines ){
    ss << line << "\n";
  }
  return ss.str();
}







////////
//LOAD//
////////

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
  current_line = graph.loadSelfNodesAndEdges( file_lines, current_line, options ) + 1;

  return "load successful";
}

}

std::string load_file(
  std::string const & filename,
  graph::Graph & graph,
  Options & options
){
  return load_file( get_file_lines( filename ), graph, options );
}


}//namespace
