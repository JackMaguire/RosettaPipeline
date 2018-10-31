#pragma once

#include <graph/Graph.fwd.hh>
#include <graph/Node.fwd.hh>

#include <sstream>
#include <string>
#include <vector>

namespace compile {

struct script_pair {
  std::string setup_script;
  std::string run_script;
};

std::string
compile( graph::Graph const & g );

std::string
just_compile_run_script( graph::Graph const & g );

std::string
setup_working_directory( std::vector< graph::NodeCSP > const & nodes_in_order );

void
compile_run_script(
  std::vector< graph::NodeCSP > const & nodes_in_order,
  std::ostream & run_script
);

//utility
bool cycleExists( graph::Graph const & g );

std::vector< graph::NodeCSP >
determineOrderOfNodes( graph::Graph const & g );

void addGlobalIntroToScript( std::stringstream & script );

void addStageIntroToScript( int stage, std::stringstream & script );

void addGlobalVariablesToRunScript( std::stringstream & script );

}//namespace compile
