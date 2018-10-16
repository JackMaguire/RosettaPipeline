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

bool Options::show_node_titles = true;
bool Options::put_node_titles_to_side = true;
int Options::grid_size = 10;

bool Options::serialize_intermediate_poses = false;
int Options::num_processors = 0;
std::string Options::default_run_command =
  std::string( "mpirun -n $nproc rosetta_scripts.mpiserialization.linuxgccrelease @ flags" );
bool Options::delete_unused_intermediate_poses = true;

};
