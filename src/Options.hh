#pragma once

#include <string>
#include <vector>

struct Options {

  //////////////
  // View Options
  bool show_node_titles;
  bool put_node_titles_to_side;
  int grid_size;

  //////////////
  // Run Options
  bool serialize_intermediate_poses;
  int num_processors;
  std::string default_run_command;
  bool delete_unused_intermediate_poses;

  ///////////
  //SAVE/LOAD
  void save( std::vector< std::string > & output_lines );
  int load( std::vector< std::string > const & lines, int line_to_start_at );

};
