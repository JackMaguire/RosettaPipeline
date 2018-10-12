#pragma once

#include <graph/Graph.fwd.hh>
#include <graph/Node.hh>
#include <graph/Edge.hh>

#include <memory>
#include <vector>

namespace graph {

struct PreliminaryEdge {
  NodeSP source_node;
  int cursor_x;
  int cursor_y;

  PreliminaryEdge( NodeSP const & source, int x, int y ) :
    source_node( source ),
    cursor_x( x ),
    cursor_y( y )
  {}
};

class Graph {
public:
  Graph();

public://general access

public://node access
  int getNumNodes() const;
  std::vector< NodeSP > const & nodes() const;
  void addNode( NodeSP const & n );

public://edge access
  int getNumEdges() const;
  std::vector< EdgeSP > const & edges() const;
  EdgeSP addEdge( NodeSP const & source, NodeSP const & destination );

protected:
  int getNextNodeID() { return next_node_id_++; }

private:
  std::vector< NodeSP > nodes_;
  std::vector< EdgeSP > edges_;

  NodeSP selected_node_;
  EdgeSP selected_edge_;

  int next_node_id_;

  //represents edges that are in the middle of being drawn
  PreliminaryEdgeSP ghost_edge_;
};


inline
int
Graph::getNumNodes() const {
  return nodes_.size();
}

inline
std::vector< NodeSP > const &
Graph::nodes() const {
  return nodes_;
}

inline
void
Graph::addNode( NodeSP const & n ) {
  n->setID( getNextNodeID() );
  nodes_.push_back( n );
}



inline
int
Graph::getNumEdges() const {
  return edges_.size();
}

inline
std::vector< EdgeSP > const &
Graph::edges() const {
  return edges_;
}

inline
EdgeSP
Graph::addEdge( NodeSP const & source, NodeSP const & destination ) {
  //NodeCSP const source = e->getSourceNodeCSP();
  //NodeCSP const destination = e->getDestinationNodeCSP();
  if( source == destination ){
    return 0;
  }

  // Make sure there is not already an edge
  for( EdgeCSP const & de : source->getDownstreamEdges() ) {
    if( de->getDestinationNodeCSP() == destination ) {
      return 0;
    }
  }

  // Make sure there is no reverse Edge
  for( EdgeCSP de : destination->getDownstreamEdges() ) {
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

}//namespace graph
