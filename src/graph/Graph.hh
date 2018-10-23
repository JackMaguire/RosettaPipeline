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
  ~Graph();

public://deletion
  void removeNodeAndDeleteItsEdges( NodeSP & );

  void removeEdgeAndNotifyItsNodes( EdgeSP & );

public://IO
  void saveSelfNodesAndEdges( std::vector< std::string > & output_lines ) const;

  void loadSelfNodesAndEdges( std::vector< std::string > & output_lines, int line_to_start_at );


public://node access
  int getNumNodes() const;
  std::vector< NodeSP > const & nodes() const;
  void addNode( NodeSP const & n );

public://edge access
  int getNumEdges() const;
  std::vector< EdgeSP > const & edges() const;
  EdgeSP addEdge( NodeSP const & source, NodeSP const & destination );

public://selection access
  NodeSP & selectedNode();
  NodeCSP selectedNode() const;
  void setSelectedNode( NodeSP const & );

  EdgeSP selectedEdge();
  EdgeCSP selectedEdge() const;
  void setSelectedEdge( EdgeSP const & );

  PreliminaryEdgeSP ghostEdge();
  PreliminaryEdgeCSP ghostEdge() const;
  void setGhostEdge( PreliminaryEdgeSP const & );

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

  if( selected_node_ == 0 ){
    setSelectedNode( n );
  }
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
NodeSP &
Graph::selectedNode() {
  return selected_node_;
}

inline
NodeCSP
Graph::selectedNode() const {
  return selected_node_;
}

inline
void
Graph::setSelectedNode( NodeSP const & n ) {
  selected_node_ = n;
  if( n != 0 ) {
    selected_edge_ = 0;
  }
}

inline
EdgeSP
Graph::selectedEdge() {
  return selected_edge_;
}

inline
EdgeCSP
Graph::selectedEdge() const {
  return selected_edge_;
}

inline
void
Graph::setSelectedEdge( EdgeSP const & e ) {
  selected_edge_ = e;
  if( e != 0 ) {
    selected_node_ = 0;
  }
}

inline
PreliminaryEdgeSP
Graph::ghostEdge() {
  return ghost_edge_;
}

inline
PreliminaryEdgeCSP
Graph::ghostEdge() const {
  return ghost_edge_;
}

inline
void
Graph::setGhostEdge( PreliminaryEdgeSP const & e ) {
  ghost_edge_ = e;
}


}//namespace graph
