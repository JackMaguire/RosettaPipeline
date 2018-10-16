#pragma once

#include <graph/Graph.hh>

namespace compile {

bool cycleExists( graph::Graph const & g );

std::string
compile_setup_script( graph::Graph const & g );

}//namespace compile
