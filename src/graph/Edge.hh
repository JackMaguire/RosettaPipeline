#pragma once

#include <graph/Edge.fwd.hh>
#include <graph/Node.fwd.hh>

namespace graph {

class Edge {
public:
  Edge();
  Edge( NodeCP const & source_node, NodeCP const & destination_node );

  ~Edge();

public://Node getters and setters
  Node const & getSourceNode() const;
  NodeCSP getSourceNodeCSP() const;
  NodeSP getSourceNodeSP();
  void setSourceNode( NodeSP const & node );
  void setSourceNode( NodeWP const & node );

  Node const & getDestinationNode() const;
  NodeCSP getDestinationNodeCSP() const;
  NodeSP getDestinationNodeSP();
  void setDestinationNode( NodeSP const & node );
  void setDestinationNode( NodeWP const & node );

public://Getter and setters for sorting options
  std::string const & columnNameToSortBy() const;
  std::string const & getColumnNameToSortBy() const;
  void setColumnNameToSortBy( std::string setting ) const;//passing by value on purpose

protected:
  void init();

private:
  NodeWP source_node_;//Weak pointer to avoid circular references
  NodeWP destination_node_;//Weak pointer to avoid circular references

  std::string column_name_to_sort_by_;
  bool positive_scores_are_better_;

  int num_results_to_transfer_;
  double percentage_of_results_to_transfer_;
  bool use_percentage_instead_of_count_;

  std::string notes_;
};

inline
Node const &
Edge::getSourceNode() const {
  auto ptr = source_node_.lock();
  assert( ptr );
  return *ptr;
}

inline
NodeCSP
Edge::getSourceNodeCSP() const {
  return source_node_.lock();
}

inline
NodeSP
Edge::getSourceNodeSP() {
  return source_node_.lock();
}

inline
void
Edge::setSourceNode( NodeSP const & node ) {
  source_node_ = node;
}

inline
void
Edge::setSourceNode( NodeWP const & node ) {
  source_node_ = node;
}

inline
Node const &
Edge::getDestinationNode() const {
  auto ptr = destination_node_.lock();
  assert( ptr );
  return *ptr;
}

inline
NodeCSP
Edge::getDestinationNodeCSP() const {
  return destination_node_.lock();
}

inline
NodeSP
Edge::getDestinationNodeSP() {
  return destination_node_.lock();
}

inline
void
Edge::setDestinationNode( NodeSP const & node ) {
  destination_node_ = node;
}

inline
void
Edge::setDestinationNode( NodeWP const & node ) {
  destination_node_ = node;
}

inline
std::string const &
Edge::columnNameToSortBy() const {
  return column_name_to_sort_by_;
}

inline
std::string const &
Edge::getColumnNameToSortBy() const {
  return column_name_to_sort_by_;
}

inline
void
Edge::setColumnNameToSortBy( std::string setting ) const {//passing by value on purpose
  column_name_to_sort_by_ = std::move( setting );
}


}//namespace graph
