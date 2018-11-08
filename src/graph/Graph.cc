#include <graph/Graph.hh>
#include <graph/Node.hh>
#include <graph/Edge.hh>

#include <iostream>

namespace graph {

Graph::Graph() :
  next_node_id_( 0 )
{}

Graph::~Graph(){}


EdgeSP
Graph::addEdge( NodeSP const & source, NodeSP const & destination ) {
  if( source == destination ){
    return 0;
  }

  // Make sure there is not already an edge
  for( EdgeCSP const & de : source->getDownstreamEdges() ) {
    if( de->getDestinationNodeCSP() == destination ) {
      return 0;
    }
  }

  // Make sure there is no reverse edge
  for( EdgeCSP const & de : destination->getDownstreamEdges() ) {
    if( de->getDestinationNodeCSP() == source ) {
      return 0;
    }
  }

  EdgeSP const new_edge = std::make_shared< Edge >( source, destination );
  source->addDownstreamEdge( new_edge );
  destination->addUpstreamEdge( new_edge );
  edges_.push_back( new_edge );

  return new_edge;
}


void
Graph::removeNodeAndDeleteItsEdges( NodeSP & n ) {

  {//Upstream
    std::vector< EdgeSP > & upstream_edges = n->getUpstreamEdges();
    for( int i = upstream_edges.size() - 1; i >= 0; --i ) {
      removeEdgeAndNotifyItsNodes( upstream_edges[ i ] );
    }
  }

  {//Downstream
    std::vector< EdgeSP > & downstream_edges = n->getDownstreamEdges();
    for( int i = downstream_edges.size() - 1; i >= 0; --i ) {
      removeEdgeAndNotifyItsNodes( downstream_edges[ i ] );
    }
  }

  auto const iter = std::find( nodes_.begin(), nodes_.end(), n );
  if( iter != nodes_.end() ){
    nodes_.erase( iter );
  }
}

void
Graph::removeEdgeAndNotifyItsNodes( EdgeSP & e ) {
  e->getDestinationNodeSP()->removeUpstreamEdge( e );
  e->getSourceNodeSP()->removeDownstreamEdge( e );

  auto const iter = std::find( edges_.begin(), edges_.end(), e );
  if( iter != edges_.end() ){
    edges_.erase( iter );
  }
}


// Save/Load
void
Graph::saveSelfNodesAndEdges( serialization::Archiver & archiver ) const {
  archiver.add_element( "START", "GRAPH" );
  archiver.add_element( "next_node_id", std::to_string( next_node_id_ ) );
  archiver.add_element( "num_nodes", std::to_string( nodes_.size() ) );

  for( NodeSP const & n : nodes_ ) {
    n->save( archiver );
  }

  archiver.add_element( "num_edges", std::to_string( edges_.size() ) );
  for( EdgeSP const & e : edges_ ) {
    e->save( archiver );
  }

  archiver.add_element( "END", "GRAPH" );
}

int
Graph::loadSelfNodesAndEdges(
  std::vector< std::string > const & lines,
  int line_to_start_at,
  Options const & options
) {
  // First, Delete everything!
  selected_node_ = 0;
  selected_edge_ = 0;
  next_node_id_ = 0;
  nodes_.clear();
  edges_.clear();

  if( lines[ line_to_start_at ] != "START_GRAPH" ) {
    //TODO throw something
  }

  int current_line = line_to_start_at;

  while( lines[ ++current_line ] != "END_GRAPH" ){

    std::string const line = lines[ current_line ];
    std::vector< std::string > tokens;
    {//stolen from https://stackoverflow.com/questions/13172158/c-split-string-by-line
      std::string const delimiter = " ";
      std::string::size_type prev = 0;
      std::string::size_type pos = line.find( delimiter, prev );
      while ( pos != std::string::npos ) {
	tokens.push_back( line.substr( prev, pos - prev ) );
	prev = pos + 1;
	pos = line.find( delimiter, prev );
      }

      // To get the last substring (or only, if delimiter is not found)
      tokens.push_back( line.substr( prev ) );
    }

    if( tokens.size() < 2 ) continue;

    if( tokens[ 0 ] == "next_node_id" ) {
      next_node_id_ = std::stoi( tokens[ 1 ] );
      continue;
    }
    if( tokens[ 0 ] == "num_nodes" ) {
      uint const num_nodes = std::stoi( tokens[ 1 ] );
      for( uint i = 0; i < num_nodes; ++i ) {
	NodeSP new_node = std::make_shared< Node >( lines, ++current_line, options );
	nodes_.emplace_back( std::move( new_node ) );
	while( lines[ ++current_line ] != "END_NODE" ){}
      }
      assert( nodes_.size() == num_nodes );
      continue;
    }
    if( tokens[ 0 ] == "num_edges" ) {
      int const num_edges = std::stoi( tokens[ 1 ] );
      for( int i = 0; i < num_edges; ++i ) {
	EdgeSP new_edge = std::make_shared< Edge >();
	new_edge->load( nodes_, lines, ++current_line );
	edges_.emplace_back( std::move( new_edge ) );
	while( lines[ ++current_line ] != "END_EDGE" ){}
      }
      continue;
    }
  }

  assert( nodes_.size() > 0 );
  selected_node_ = nodes_[ 0 ];

  return current_line;
}// load self


}//namesapce graph
