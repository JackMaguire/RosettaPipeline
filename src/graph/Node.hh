#pragma once

#include <graph/Node.fwd.hh>
#include <graph/Edge.fwd.hh>
#include <graph/Graph.fwd.hh>

#include <serialization.hh>

#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

#include <Options.hh>

namespace graph {

class Node : public std::enable_shared_from_this< Node > {

friend class Graph;

public:
  Node( Options const &, int x, int y, Graph * parent );
  Node( Options const &, std::string title, int x, int y, Graph * parent );//pass-by-value on purpose
  ~Node();

  virtual std::string
  uniqueToken() const {
    return "RS_NODE";
  }

public://getters and setters:
  int ID() const { return id_; }
  void setID( int id );

  int X() const { return x_; }
  void setX( int x ){ x_ = x; }

  int Y() const { return y_; }
  void setY( int y ){ y_ = y; }
  
  bool useDefaultCommand() const { return use_default_command_; }
  void setUseDefaultCommand( bool setting ){ use_default_command_ = setting; }

  std::string const & command() const { return command_; }
  void setCommand( std::string command ){ command_ = std::move( command ); }

  std::string const & title() const { return title_; }
  void setTitle( std::string title );

  bool useScriptFile() const { return use_script_file_; }
  void setUseScriptFile( bool setting ){ use_script_file_ = setting; }

  std::string const & xmlScriptFilename() const { return xml_script_filename_; }
  void setXMLScriptFilename( std::string setting ){ xml_script_filename_ = std::move( setting ); }

  std::string const & xmlScript() const { return xml_script_; }
  void setXMLScript( std::string setting ){ xml_script_ = std::move( setting ); }  

  int stage() const { return stage_; }//TODO check validity
  void setStage( int setting ) const { stage_ = setting; }
  void setStageValidity( bool setting ) const { stage_is_valid_ = setting; }

  std::string const & notes() const { return notes_; }
  void setNotes( std::string setting ){ notes_ = std::move( setting ); }

public://general compilation access
  std::string const & getEffectiveCommand( Options const & options ) const;

  int inDegreeIgnoringTheseNodes( std::vector< NodeCSP > const & nodes_to_ignore ) const;

  std::string dirname() const;

  std::string determineAutoFlags() const;

  static std::string commonFlags();

  std::string getAllRosettaFlags() const;

public://edge access and modification
  int numUpstreamEdges() const;

  EdgeSP getUpstreamEdge( int index );

  EdgeCSP getUpstreamEdge( int index ) const;

  void addUpstreamEdge( EdgeSP const & edge );

  void removeUpstreamEdge( EdgeSP const & edge );
  
  int numDownstreamEdges() const;

  EdgeSP getDownstreamEdge( int index );

  EdgeCSP getDownstreamEdge( int index ) const;

  void addDownstreamEdge( EdgeSP const & edge );

  void removeDownstreamEdge( EdgeSP const & edge );

  Graph * parent() const { return parent_; }

public://container access
  std::vector< EdgeSP > & getUpstreamEdges() { return upstream_edges_; }
  std::vector< EdgeSP > const & getUpstreamEdges() const { return upstream_edges_; }

  std::vector< EdgeSP > & getDownstreamEdges() { return downstream_edges_; }
  std::vector< EdgeSP > const & getDownstreamEdges() const { return downstream_edges_; }

  std::string const & userRosettaFlags() const { return user_rosetta_flags_; }
  void setUserRosettaFlags( std::string new_flags ){
    user_rosetta_flags_ = std::move( new_flags );
  }

protected:
  void init( Options const & );

  void save( serialization::Archiver & archiver ) const;

public://This technically has to be public in order to use make_shared
  //load ctor
  Node(
    serialization::Unarchiver &,
    Options const &,
    Graph * parent
  );

private:
  int id_;
  int x_;
  int y_;

  std::vector< EdgeSP > upstream_edges_;
  std::vector< EdgeSP > downstream_edges_;

  bool use_default_command_;
  std::string command_;
  std::string title_;
  bool still_using_default_title_;

  bool use_script_file_;
  std::string xml_script_filename_;
  std::string xml_script_;

  // The graph parser will assign a stage to this node, set stage_is_valid_ to
  // true, run methods that call stage_, and set stage_is_valid_ to false.
  // stage_is_valid_ is meant to prevent other methods from calling getStage()
  // and assuming it is the current stage when it is in fact unassigned
  mutable int stage_;
  mutable bool stage_is_valid_;

  std::string user_rosetta_flags_;
  std::string notes_;

  Graph * parent_;
};

inline
std::string const &
Node::getEffectiveCommand( Options const & options ) const {
  if( use_default_command_ ) {
    return options.default_run_command;
  } else {
    return command_;
  }
}


inline
int Node::numUpstreamEdges() const {
  return upstream_edges_.size();
};

inline
EdgeSP Node::getUpstreamEdge( int index ) {
#ifndef NDEBUG
  //TODO bounds checking?
  //std::vector does debug bounds checking anyways so we might not bother
#endif
  return upstream_edges_[ index ];
}

inline
EdgeCSP Node::getUpstreamEdge( int index ) const {
  return upstream_edges_[ index ];
}

inline
void Node::addUpstreamEdge( EdgeSP const & edge ){
  upstream_edges_.push_back( edge );
}

inline
void Node::removeUpstreamEdge( EdgeSP const & edge ) {
  auto const iter = std::find( upstream_edges_.begin(), upstream_edges_.end(), edge );
  if( iter != upstream_edges_.end() ){
    upstream_edges_.erase( iter );
  }
}


inline
int Node::numDownstreamEdges() const {
  return downstream_edges_.size();
};

inline
EdgeSP Node::getDownstreamEdge( int index ) {
#ifndef NDEBUG
  //TODO bounds checking?
  //std::vector does debug bounds checking anyways so we might not bother
#endif
  return downstream_edges_[ index ];
}

inline
EdgeCSP Node::getDownstreamEdge( int index ) const {
  return downstream_edges_[ index ];
}

inline
void Node::addDownstreamEdge( EdgeSP const & edge ){
  downstream_edges_.push_back( edge );
}

inline
void Node::removeDownstreamEdge( EdgeSP const & edge ) {
  auto const iter = std::find( downstream_edges_.begin(), downstream_edges_.end(), edge );
  if( iter != downstream_edges_.end() ){
    downstream_edges_.erase( iter );
  }
}

inline
void
Node::setID( int id ){
  id_ = id;
  if( still_using_default_title_ ){
    title_ = "Node_" + std::to_string( id_ );
  }
}

inline
void
Node::setTitle( std::string title ){
  title_ = std::move( title );
  still_using_default_title_ = false;
}  

}//namespace graph
