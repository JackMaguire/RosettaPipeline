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

  for( ExtraFile const & file : extra_files_ ){
    archiver.add_element( "extra_file_name", file.name );
    archiver.add_element( "extra_file_contents", file.contents );
  }

  archiver.add_element( "END", "GRAPH" );
}

void
Graph::loadSelfNodesAndEdges(
  serialization::Unarchiver & unarchiver,
  Options const & options
) {
  {
    serialization::ArchiveElement first_element = unarchiver.get_next_element();
    assert( first_element.token == "START" );
    assert( first_element.value == "GRAPH" );
  }

  // First, Delete everything!
  selected_node_ = 0;
  selected_edge_ = 0;
  next_node_id_ = 0;
  nodes_.clear();
  edges_.clear();

  for( serialization::ArchiveElement element = unarchiver.get_next_element();
       element.token != "END" || element.value != "GRAPH";
       element = unarchiver.get_next_element() ){

    if( element.token == "next_node_id" ){
      next_node_id_ = std::stoi( element.value );
      continue;
    }

    if( element.token == "extra_file_name" ){
      std::string name = element.value;
      element = unarchiver.get_next_element();
      assert( element.token == "extra_file_contents" );
      extra_files_.emplace_back( std::move( name ), element.value );
      continue;
    }


    if( element.token == "num_nodes" ){
      uint const num_nodes = std::stoi( element.value );
      for( uint i = 0; i < num_nodes; ++i ) {
	serialization::ArchiveElement node_element = unarchiver.get_next_element();
	assert( node_element.token == "START" );
	NodeSP new_node = std::make_shared< Node >( unarchiver, options );
	nodes_.emplace_back( std::move( new_node ) );
      }
      continue;
    }

    if( element.token == "num_edges" ){
      uint const num_edges = std::stoi( element.value );
      for( uint i = 0; i < num_edges; ++i ) {
	serialization::ArchiveElement edge_element = unarchiver.get_next_element();
	assert( edge_element.token == "START" );
	assert( edge_element.value == "EDGE" );
	EdgeSP new_edge = std::make_shared< Edge >();
	new_edge->load( nodes_, unarchiver );
	edges_.emplace_back( std::move( new_edge ) );
      }
      continue;
    }

  }

  assert( nodes_.size() > 0 );
  selected_node_ = nodes_[ 0 ];
}// load self


}//namesapce graph
