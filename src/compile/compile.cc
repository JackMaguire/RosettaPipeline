#include <compile/compile.hh>

#include <graph/Graph.hh>
#include <graph/Node.hh>
#include <graph/Edge.hh>

#include <global_data/options.hh>
#include <global_data/version.hh>

#include <set>
#include <sstream>

using namespace graph;

namespace compile {

script_pair
compile( graph::Graph const & g ){
  script_pair scripts;

  if( cycleExists( g ) ){
    scripts.setup_script = "Error: could not compile, cycle exists in graph";
    scripts.run_script = scripts.setup_script;
    return scripts;
  }

  auto const nproc = global_data::Options::num_processors;

  if( nproc == 0 ){
    scripts.setup_script = "Error: could not compile, num_processors value is not set."
      "Please go to the 'Options' tab and select a number larger than 0";
    scripts.run_script = scripts.setup_script;
    return scripts;
  }

  if( nproc < 0 ){
    scripts.setup_script = std::to_string( nproc ) +
      "is an invalid value for num_processors. Please set this to a positive number.";
    scripts.run_script = scripts.setup_script;
    return scripts;
  }



  std::vector< graph::NodeCSP > const nodes_in_order = determineOrderOfNodes( g );

  for( uint stage = 1; stage <= nodes_in_order.size(); ++stage ) {
    nodes_in_order[ stage - 1 ]->setStage( stage );
    nodes_in_order[ stage - 1 ]->setStageValidity( true );
  }
  scripts.setup_script = compile_setup_script( nodes_in_order ); 
  scripts.run_script = compile_run_script( nodes_in_order ); 
  for( graph::NodeCSP const & node : nodes_in_order ) {
    node->setStageValidity( false );
  }

  return scripts;
}

std::string
compile_setup_script( std::vector< graph::NodeCSP > const & nodes_in_order ){
  std::stringstream script;

  addGlobalIntroToScript( script );
  
  for( graph::NodeCSP const & node : nodes_in_order ){
    addStageIntroToScript( node->stage(), script );

    std::string const dirname = node->dirname();
    script << "mkdir " << dirname << "\n"
      "cd " << dirname << "\n";

    if( node->numUpstreamEdges() > 0 ) {
      script << "touch input_files\n";
    }

    for( auto const & flag : node->getAllRosettaFlags() ) {
      script << "echo \"" << flag << "\" >> flags\n";
    }

    if( ! node->useScriptFile() ) {
      // Write out custom file
      std::string xml_script = node->xmlScript();

      {//replace \n with \\n
	std::string const search = "\n";
	std::string const replace = "\\n";

	size_t pos = xml_script.find( search, 0 );
	while( pos != std::string::npos ) {
	  xml_script.replace( pos, search.length(), replace );
	  pos += replace.length();
	  pos = xml_script.find( search, pos );
	}
      }

      {//replace ' with "
	std::string const search = "'";
	std::string const replace = "\"";

	size_t pos = xml_script.find( search, 0 );
	while( pos != std::string::npos ) {
	  xml_script.replace( pos, search.length(), replace );
	  pos += replace.length();
	  pos = xml_script.find( search, pos );
	}
      }

      script << "printf " << xml_script << "script.xml\n";
    }

    if( global_data::Options::serialize_intermediate_poses ) {
      if( node->numUpstreamEdges() > 0 ) {
	script << "echo \"-in:file:srlz_override 1\" >> flags\n";
      }

      if( node->numDownstreamEdges() > 0 ) {
	script << "echo \"-out:file:srlz 1\" >> flags\n";
      }
    }

    script << "cd ../\n";
  }

  return script.str();
}

std::string
compile_run_script( std::vector< graph::NodeCSP > const & nodes_in_order ){
  std::stringstream script;

  addGlobalIntroToScript( script );
  
  for( graph::NodeCSP const & node : nodes_in_order ){
    addStageIntroToScript( node->stage(), script );
  }

  return script.str();
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

void addGlobalIntroToScript( std::stringstream & script ) {
  script << "# Script was created using JD3BASH\n";
  script << "# Version number: " << global_data::Version::number << "\n";
  script << "# Visit github.com/JackMaguire/JD3Bash for details\n\n";
}

void addStageIntroToScript( int stage, std::stringstream & script ) {
  script << "\n###########\n";
  script << "# STAGE " << stage << " #\n";
  script << "###########\n\n";
}

void addGlobalVariablesToRunScript( std::stringstream & script ) {
  script << "nproc=" << global_data::Options::num_processors << "\n";
}


}//namespace compile
