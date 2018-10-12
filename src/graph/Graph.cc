#include <graph/Graph.hh>

namespace graph {

Graph::Graph(){}

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

}//namesapce graph
