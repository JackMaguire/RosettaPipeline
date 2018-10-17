#pragma once

#include <string>
#include <vector>

namespace global_data {

struct Options {

  //////////////
  // View Options
  static bool show_node_titles;
  static bool put_node_titles_to_side;
  static int grid_size;

  //////////////
  // Run Options
  static bool serialize_intermediate_poses;
  static int num_processors;
  static std::string default_run_command;
  static bool delete_unused_intermediate_poses;

  ///////////
  //SAVE/LOAD
  static void save( std::vector< std::string > & output_lines );
  static void load( std::vector< std::string > const & lines, int line_to_start_at );

};

}//namespace global_data
