#pragma once

#include <graph/Edge.fwd.hh>

#include <vector>

namespace graph {

class Node {

private:
  int id_;
  int x_;
  int y_;

  //Colors:
  int red_;
  int green_;
  int blue_;

  std::vector< EdgeSP > upstream_edges_;
  std::vector< EdgeSP > downstream_edges_;

  bool use_default_command_;
  std::string command_;
  std::string title_;

  bool use_script_file_;
  std::string xml_script_filename_;
  std::string xml_script_;

  // The graph parser will assign a stage to this node, set stage_is_valid_ to
  // true, run methods that call stage_, and set stage_is_valid_ to false.
  // stage_is_valid_ is meant to prevent other methods from calling getStage()
  // and assuming it is the current stage when it is in fact unassigned
  int stage_;
  bool stage_is_valid_;

  std::vector< std::string > user_rosetta_flags_;
  std::string notes_;
};

}//namespace graph
