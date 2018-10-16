#include <graph/Graph.hh>

#include <graph/Graph.hh>
#include <graph/Node.hh>
#include <graph/Edge.hh>

#include <set>

using namespace graph;

namespace compile {

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

std::string
compile_setup_script( graph::Graph const & g ){
  std::string script = "";

  return script;
}

};
