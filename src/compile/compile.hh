#pragma once

#include <graph/Graph.fwd.hh>
#include <graph/Node.fwd.hh>

#include <sstream>
#include <string>
#include <vector>

#include <Options.hh>

namespace compile {

struct CompilationResult {
  CompilationResult( bool s, std::string r ) :
    success( s ),
    result( std::move( r ) )
  {}

  bool success;
  std::string result;
};

struct script_pair {
  std::string setup_script;
  std::string run_script;
};

CompilationResult
compile( graph::Graph const &, Options const & );

std::string
just_compile_run_script( graph::Graph const &, Options const & );

std::string
setup_working_directory( std::vector< graph::NodeCSP > const & nodes_in_order, Options const & );

void
compile_run_script(
  std::vector< graph::NodeCSP > const & nodes_in_order,
  Options const &,
  std::ostream & run_script
);

//utility
bool cycleExists( graph::Graph const & g );

std::vector< graph::NodeCSP >
determineOrderOfNodes( graph::Graph const & g );

void addGlobalIntroToScript( std::ostream & run_script );

void addStageIntroToScript( Node const & node_for_stage, std::ostream & run_script );

void addGlobalVariablesToRunScript( std::ostream & run_script, Options const & );

}//namespace compile
