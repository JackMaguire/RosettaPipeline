#pragma once

#include <string>
#include <vector>

namespace options {

struct GlobalOptions {

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

bool GlobalOptions::show_node_titles = true;
bool GlobalOptions::put_node_titles_to_side = true;
int GlobalOptions::grid_size = 10;

bool GlobalOptions::serialize_intermediate_poses = false;
int GlobalOptions::num_processors = 0;
std::string GlobalOptions::default_run_command =
  std::string( "mpirun -n $nproc rosetta_scripts.mpiserialization.linuxgccrelease @ flags" );
bool GlobalOptions::delete_unused_intermediate_poses = true;

};
