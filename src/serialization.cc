#include <serialization.hh>

#include <graph/Graph.hh>

#include <iostream>
#include <vector>
#include <sstream>


namespace serialization {

std::string save(
  Options const & options,
  graph::Graph const & graph
) {
  std::vector< std::string > save_lines;
  options->save( save_lines );
  graph->saveSelfNodesAndEdges( save_lines );
    
  std::stringstream ss;
  for( std::string const & line : save_lines ){
    ss << line << "\n";
  }
  return ss.str();
}

}//namespace
