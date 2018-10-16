#pragma once

#include <graph/Graph.fwd.hh>

#include <sstream>
#include <string>
#include <vector>

namespace compile {

std::string
compile_setup_script( graph::Graph const & g );


//utility
bool cycleExists( graph::Graph const & g );

std::vector< graph::NodeCSP >
determineOrderOfNodes( Graph const & g );

void addGlobalIntroToScript( std::stringstream & script );

void addStageIntroToScript( int stage, std::stringstream & script );

void addGlobalVariablesToRunScript( std::stringstream & script );

}//namespace compile
