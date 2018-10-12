#pragma once

#include <graph/Edge.fwd.hh>
#include <graph/Node.fwd.hh>

#include <string>

namespace graph {

class Edge {
public:
  Edge();
  Edge( NodeSP const & source_node, NodeSP const & destination_node );

  ~Edge();

public://Node getters and setters
  Node const & sourceNode() const;
  Node const & getSourceNode() const;
  NodeCSP getSourceNodeCSP() const;
  NodeSP getSourceNodeSP();
  void setSourceNode( NodeSP const & node );
  void setSourceNode( NodeWP const & node );

  Node const & destinationNode() const;
  Node const & getDestinationNode() const;
  NodeCSP getDestinationNodeCSP() const;
  NodeSP getDestinationNodeSP();
  void setDestinationNode( NodeSP const & node );
  void setDestinationNode( NodeWP const & node );

public://Getters and setters for sorting options
  std::string const & columnNameToSortBy() const;
  std::string const & getColumnNameToSortBy() const;
  void setColumnNameToSortBy( std::string setting );//passing by value on purpose

  bool positiveScoresAreBetter() const;
  bool getPositiveScoresAreBetter() const;
  void setPositiveScoresAreBetter( bool setting );

public://Getters and setters for transfer options
  int numResultsToTransfer() const;
  int getNumResultsToTransfer() const;
  void setNumResultsToTransfer( int setting );

  double percentageOfResultsToTransfer() const;
  double getPercentageOfResultsToTransfer() const;
  void setPercentageOfResultsToTransfer( double setting );

  bool usePercentageInsteadOfCount() const;
  bool getUsePercentageInsteadOfCount() const;
  void setUsePercentageInsteadOfCount( bool setting );

public://Getters and setters for "other"
  std::string const & notes() const;
  std::string const & getNotes() const;
  void setNotes( std::string setting );//passing by value on purpose

protected:
  void init();

  void save( std::vector< std::string > & output_lines ) const;

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
Edge::sourceNode() const {
  auto ptr = source_node_.lock();
  assert( ptr );
  return *ptr;
}

inline
Node const &
Edge::getSourceNode() const {
  return sourceNode();
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
Edge::destinationNode() const {
  auto ptr = destination_node_.lock();
  assert( ptr );
  return *ptr;
}

inline
Node const &
Edge::getDestinationNode() const {
  return destinationNode();
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
Edge::setColumnNameToSortBy( std::string setting ) {//passing by value on purpose
  column_name_to_sort_by_ = std::move( setting );
}

inline
bool
Edge::positiveScoresAreBetter() const {
  return positive_scores_are_better_;
}

inline
bool
Edge::getPositiveScoresAreBetter() const {
  return positive_scores_are_better_;
}

inline
void
Edge::setPositiveScoresAreBetter( bool setting ) {
  positive_scores_are_better_ = setting;
}

inline
int
Edge::numResultsToTransfer() const{
  return num_results_to_transfer_;
}

inline
int
Edge::getNumResultsToTransfer() const {
  return num_results_to_transfer_;
}

inline
void
Edge::setNumResultsToTransfer( int setting ) {
  num_results_to_transfer_ = setting;
}

inline
double
Edge::percentageOfResultsToTransfer() const {
  return percentage_of_results_to_transfer_;
}

inline
double
Edge::getPercentageOfResultsToTransfer() const {
  return percentage_of_results_to_transfer_;
}

inline
void
Edge::setPercentageOfResultsToTransfer( double setting ) {
  percentage_of_results_to_transfer_ = setting;
}

inline
bool
Edge::usePercentageInsteadOfCount() const {
  return use_percentage_instead_of_count_;
}

inline
bool
Edge::getUsePercentageInsteadOfCount() const {
  return use_percentage_instead_of_count_;
}

inline
void
Edge::setUsePercentageInsteadOfCount( bool setting ) {
  use_percentage_instead_of_count_ = setting;
}

inline
std::string const &
Edge::notes() const {
  return notes_;
}

inline
std::string const &
Edge::getNotes() const {
  return notes_;
}

inline
void
Edge::setNotes( std::string setting ) {//passing by value on purpose
  notes_ = std::move( setting );
}

}//namespace graph
