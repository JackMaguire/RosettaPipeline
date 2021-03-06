#pragma once

#include <graph/Graph.fwd.hh>
#include <graph/Node.hh>
#include <graph/Edge.hh>

#include <serialization.hh>
#include <listeners.hh>

#include <memory>
#include <vector>

namespace graph {

class ExtraFile : public listeners::Changable< EXTRA_FILE_EDIT >{
public:
  ExtraFile(){}

  ExtraFile(
    std::string const & n,
    std::string const & c
  ) :
    name( n ),
    contents( c )
  {}

public:
  void setName( std::string n ){
    name = std::move( n );
    announce_change( Identifier< EXTRA_FILE_EDIT >() );
  }

  std::string const & getName() const {
    return name;
  }

  void setContents( std::string c ){
    contents = std::move( c );
    announce_change( Identifier< EXTRA_FILE_EDIT >() );
  }

  std::string const & getContents() const {
    return contents;
  }

private:
  std::string name;
  std::string contents;
};

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

class SelectionChangeListener {
public:
  virtual void noteChangeInSelection() = 0;
};

class Graph {
public:
  Graph();
  ~Graph();

public://deletion
  void removeNodeAndDeleteItsEdges( NodeSP & );

  void removeEdgeAndNotifyItsNodes( EdgeSP & );

public://IO
  void saveSelfNodesAndEdges( serialization::Archiver & archiver ) const;

  void loadSelfNodesAndEdges(
    serialization::Unarchiver &,
    Options const &
  );

public://node access
  int getNumNodes() const;
  std::vector< NodeSP > const & nodes() const;
  void addNode( NodeSP const & n );

public://edge access
  int getNumEdges() const;
  std::vector< EdgeSP > const & edges() const;
  EdgeSP addEdge( NodeSP const & source, NodeSP const & destination );

public://selection access
  void registerNewChangeListener( SelectionChangeListener * );

  NodeSP & selectedNode();
  NodeCSP selectedNode() const;
  void setSelectedNode( NodeSP const & );

  EdgeSP & selectedEdge();
  EdgeCSP selectedEdge() const;
  void setSelectedEdge( EdgeSP const & );

  PreliminaryEdgeSP ghostEdge();
  PreliminaryEdgeCSP ghostEdge() const;
  void setGhostEdge( PreliminaryEdgeSP const & );

public://extra files
  std::vector< ExtraFileSP > & extraFiles() {
    return extra_files_;
  }

  std::vector< ExtraFileSP > const & extraFiles() const {
    return extra_files_;
  }

  void addExtraFile(
    std::string const & name,
    std::string const & contents
  ){
    extra_files_.emplace_back( std::make_shared< ExtraFile >( name, contents ) );
  }

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

  std::vector< SelectionChangeListener * > change_listeners_;

  std::vector< ExtraFileSP > extra_files_;
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

  for( SelectionChangeListener * ptr : change_listeners_ ){
    ptr->noteChangeInSelection();
  }
}

inline
EdgeSP &
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

  for( SelectionChangeListener * ptr : change_listeners_ ){
    ptr->noteChangeInSelection();
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

inline
void
Graph::registerNewChangeListener( SelectionChangeListener * ptr ) {
  change_listeners_.push_back( ptr );
}


}//namespace graph
