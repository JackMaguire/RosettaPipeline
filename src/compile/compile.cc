#include <compile/compile.hh>

#include <graph/Graph.hh>
#include <graph/Node.hh>
#include <graph/Edge.hh>

#include <global_data/version.hh>

#include <util.hh>

#include <set>
#include <sstream>
//#include <stdlib.h>
#include <iostream>
//#include <stdio.h>//remove
#include <filesystem>//remove
#include <cstdlib>//system
#include <fstream>


using namespace graph;

namespace compile {

CompilationResult
compile( graph::Graph const & g, Options const & options ){

  if( cycleExists( g ) ){
    return CompilationResult( false, "Error: could not compile, cycle exists in graph" );
  }

  auto const nproc = options.num_processors;

  if( nproc == 0 ){
    return CompilationResult( false, "Error: could not compile, num_processors value is not set."
      "Please go to the 'Options' tab and select a number larger than 0" );
  }

  if( nproc < 0 ){
    return CompilationResult( false, std::to_string( nproc ) +
      "is an invalid value for num_processors. Please set this to a positive number." );
  }

  std::vector< graph::NodeCSP > const nodes_in_order = determineOrderOfNodes( g );

  for( uint stage = 1; stage <= nodes_in_order.size(); ++stage ) {
    nodes_in_order[ stage - 1 ]->setStage( stage );
    nodes_in_order[ stage - 1 ]->setStageValidity( true );
  }

  std::string const directory_name = setup_working_directory( nodes_in_order, options, g );
  std::string const subdirectory_name = directory_name + "/rosetta_pipeline";

  {//run script
    std::string const run_script_filename = subdirectory_name + "/run.sh";
    std::ofstream run_script;
    run_script.open( run_script_filename );
    compile_run_script( nodes_in_order, options, run_script );
    run_script.close();
  }

  for( graph::NodeCSP const & node : nodes_in_order ) {
    node->setStageValidity( false );
  }

  std::string const tar_file = directory_name + "/rosetta_pipeline.tar.gz";
  std::string const command = "tar -czf " + tar_file + " -C " + directory_name + "/rosetta_pipeline .";
  int const system_result = std::system( command.c_str() );
  assert( system_result == 0 );


  std::string tar_file_contents;
  {
    std::ifstream in( tar_file, std::ios::in | std::ios::binary );
    assert( in.is_open() );
    std::ostringstream contents;
    contents << in.rdbuf();
    in.close();
    tar_file_contents = contents.str();
  }
 
#ifdef DEBUG_SERVER_COMPILATION
  std::cout << "Check here for " << directory_name << std::endl;
  return CompilationResult( false, "compiled with DEBUG_SERVER_COMPILATION. Check " + directory_name );
#endif

  std::filesystem::remove_all( directory_name );
  return CompilationResult( true, tar_file_contents );
}

std::string
just_compile_run_script( graph::Graph const & g, Options const & options ) {
  if( cycleExists( g ) ){
    return "Error: could not compile, cycle exists in graph";
  }

  auto const nproc = options.num_processors;

  if( nproc == 0 ){
    return "Error: could not compile, num_processors value is not set."
      "Please go to the 'Options' tab and select a number larger than 0";
  }

  if( nproc < 0 ){
    return std::to_string( nproc ) +
      "is an invalid value for num_processors. Please set this to a positive number.";
  }


  std::vector< graph::NodeCSP > const nodes_in_order = determineOrderOfNodes( g );

  for( uint stage = 1; stage <= nodes_in_order.size(); ++stage ) {
    nodes_in_order[ stage - 1 ]->setStage( stage );
    nodes_in_order[ stage - 1 ]->setStageValidity( true );
  }

  std::stringstream run_script;
  compile_run_script( nodes_in_order, options, run_script );

  for( graph::NodeCSP const & node : nodes_in_order ) {
    node->setStageValidity( false );
  }

  return run_script.str();
}


std::string
setup_working_directory(
  std::vector< graph::NodeCSP > const & nodes_in_order,
  Options const & options,
  graph::Graph const & graph
){
  std::string directory_name = "/tmp/" + util::generate_random_string( 16 );
  while( std::filesystem::exists( directory_name ) ){
    directory_name = "/tmp/" + util::generate_random_string( 16 );
  }
  std::filesystem::create_directory( directory_name );
  std::cout << "creating " << directory_name << std::endl;

  std::string const subdirectory_name = directory_name + "/rosetta_pipeline";
  std::filesystem::create_directory( subdirectory_name );

  for( graph::NodeCSP const & node : nodes_in_order ){
    std::string const subsubdirectory = subdirectory_name + "/" + node->dirname();
    std::filesystem::create_directory( subsubdirectory );

    {//xml script
      std::stringstream run_script;
      run_script << "#!/bin/bash\n\n";
      node->addToRunScript( options, run_script );
      for( EdgeSP const & de : node->getDownstreamEdges() ) {
	de->addToRunScript();
      }

      std::string const script_filename = subsubdirectory + "/run.sh";
      std::ofstream script_file;
      script_file.open( script_filename );
      script_file << run_script << "\n";
      script_file.close();
    }

    {//xml script
      std::string const & xml_script = node->xmlScript();
      std::string const script_filename = subsubdirectory + "/script.xml";
      std::ofstream script_file;
      script_file.open( script_filename );
      script_file << xml_script << "\n";
      script_file.close();
    }

    {//flags
      //std::stringstream ss( node->getAllRosettaFlags() );
      std::string const all_flags = node->getAllRosettaFlags();
      std::string const flags_filename = subsubdirectory + "/flags";
      std::ofstream flags_file;
      flags_file.open( flags_filename );
      flags_file << all_flags << "\n";

      if( options.serialize_intermediate_poses ) {
	if( node->numUpstreamEdges() > 0 ) {
	  flags_file << "-in:file:srlz_override 1\n";
	}

	if( node->numDownstreamEdges() > 0 ) {
	  flags_file << "-out:file:srlz 1\n";
	}
      }
      
      flags_file.close();
    }

    if( node->numUpstreamEdges() > 0 ) {//input files
      //create empty file
      std::string const input_filename = subsubdirectory + "/input_files";
      std::ofstream input_file;
      input_file.open( input_filename );
      input_file << "";
      input_file.close();
    }    
  }

  for( graph::ExtraFileSP const & extra_file : graph.extraFiles() ){
    std::string const full_filename = subdirectory_name + "/" + extra_file->getName();
    if( ! std::filesystem::exists( full_filename ) ){
      std::ofstream file;
      file.open( full_filename );
      file << extra_file->getContents();
      file.close();
    }
  }

  return directory_name;
}

void
compile_run_script(
  std::vector< graph::NodeCSP > const & nodes_in_order,
  Options const & options,
  std::ostream & run_script
){

  addGlobalIntroToScript( run_script );
  addGlobalVariablesToRunScript( run_script, options );
  
  for( graph::NodeCSP const & node : nodes_in_order ){
    addStageIntroToScript( * node, run_script );

    std::string const dirname = node->dirname();
    run_script << "cd " << dirname << "\n";

    // THE COMMAND

    run_script << "if bash run.sh ;then \n"
      "    echo \"Done running " << dirname << "\" >> ../JD3BASH_runlog.txt\n"
      "else\n"
      "    echo \"Failed to run " << dirname << "\" >> ../JD3BASH_runlog.txt\n"
      "    exit " << node->stage() << "\n"
      "fi\n";

    if( options.delete_unused_intermediate_poses && node->numDownstreamEdges() > 0 ) {
      run_script << "\n# Delete poses not needed for future stages\n" << "TODO";
    }

    run_script << "\ncd ..\n";
    run_script << "echo \"Done With " << dirname << "\" >> JD3BASH_runlog.txt\n";
  }
}

bool cycleExists( NodeCSP const & starting_node, std::set< NodeCSP > & nodes_already_visited ) {
  if( nodes_already_visited.find( starting_node ) != nodes_already_visited.end() ) {
    return true;
  }

  auto const iter_bool_pair = nodes_already_visited.insert( starting_node );

  for( EdgeSP const & e : starting_node->getDownstreamEdges() ) {
    if( cycleExists( e->getDestinationNodeCSP(), nodes_already_visited ) ) {
      return true;
    }
  }

  nodes_already_visited.erase( iter_bool_pair.first );

  return false;
}

bool cycleExists( Graph const & g ) {
  // Recursion
  // Can use unordered_set, but this is so small it likely won't make a difference
  std::set< NodeCSP > nodes_already_visited;
  for( NodeSP const & n : g.nodes() ) {
    if( cycleExists( n, nodes_already_visited ) ) {
      return true;
    }
  }
  return false;
}


std::vector< graph::NodeCSP > determineOrderOfNodes( graph::Graph const & g ) {
  std::vector< graph::NodeSP > unassigned_nodes = g.nodes();//Copy!
  std::vector< graph::NodeCSP > assigned_nodes_in_order;

  while ( true ) {
    // Add nodes that do not depend on any unassigned node
    // Work backwards so that we can easily delete while we work
    for( int j = unassigned_nodes.size() - 1; j >= 0; --j ) {
      graph::NodeCSP const & u_node = unassigned_nodes[ j ];
      if( u_node->inDegreeIgnoringTheseNodes( assigned_nodes_in_order ) == 0 ) {
	assigned_nodes_in_order.push_back( u_node );
	unassigned_nodes.erase( unassigned_nodes.begin() + j );
      }
    }

    if( unassigned_nodes.size() == 0 ) {
      break;
    }
  }

  return assigned_nodes_in_order;
}

void addGlobalIntroToScript( std::ostream & run_script ) {
  run_script << "# Script was created using RosettaPipeline\n";
  run_script << "# Version number: " << global_data::Version::number << "\n";
  run_script << "# Visit github.com/JackMaguire/RosettaPipeline for details\n\n";
}

void addStageIntroToScript( graph::Node const & node, std::ostream & run_script ) {
  run_script << "\n##########################\n";
  run_script << "# STAGE " << node.stage() << ": " << node.title()  << "\n";
  run_script << "#\n#\n";
}

void addGlobalVariablesToRunScript( std::ostream & run_script, Options const & options ) {
  run_script << "nproc=" << options.num_processors << "\n";
}


}//namespace compile
