#pragma once

#include <graph/Graph.hh>

#include <string>
#include <vector>

namespace compile {

std::string
compile_setup_script( graph::Graph const & g );


//utility
bool cycleExists( graph::Graph const & g );

std::vector< graph::NodeCSP >
determineOrderOfNodes( Graph const & g );


}//namespace compile
