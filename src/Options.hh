#pragma once

#include <string>
#include <vector>
#include <memory>

struct Options {

  Options(){
    show_node_titles = true;
    put_node_titles_to_side = true;
    grid_size = 10;

    serialize_intermediate_poses = false;
    num_processors = 0;
    default_run_command =
      std::string( "mpirun -n $nproc rosetta_scripts.mpiserialization.linuxgccrelease @ flags" );
    delete_unused_intermediate_poses = true;
  }

  ~Options();

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
  void save( std::vector< std::string > & output_lines ) const;
  int load( std::vector< std::string > const & lines, int line_to_start_at );

};

using OptionsSP = std::shared_ptr< Options >;
using OptionsCSP = std::shared_ptr< const Options >;
using OptionsWP = std::weak_ptr< Options >;
using OptionsCWP = std::weak_ptr< const Options >;

class OptionsHolder {
public:
  OptionsHolder(){}

  OptionsHolder( OptionsSP options ){
    options_ = std::move( options );
  }

  virtual ~OptionsHolder(){}

  void setOptions( OptionsSP options ){
    options_ = std::move( options );
  }

protected:
  OptionsSP options_;
};
