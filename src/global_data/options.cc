#include <global_data/options.hh>

namespace options {

////////////
// Save/Load
void GlobalOptions::save( std::vector< std::string > & output_lines ) {
  output_lines.emplace_back( "START_OPTIONS" );
  output_lines.emplace_back( "show_node_titles " + std::to_string( show_node_titles_ ) );
  output_lines.emplace_back( "put_node_titles_to_side " + std::to_string( put_node_titles_to_side_ ) );
  output_lines.emplace_back( "num_processors " + std::to_string( num_processors_ ) );
  output_lines.emplace_back( "default_run_command " + std::to_string( default_run_command_ ) );
  output_lines.emplace_back( "delete_unused_intermediate_poses " + std::to_string( delete_unused_intermediate_poses_ ) );
  output_lines.emplace_back( "grid_size " + std::to_string( grid_size_ ) );
  output_lines.emplace_back( "serialize_intermediate_poses " + std::to_string( serialize_intermediate_poses_ ) );
  output_lines.emplace_back( "END_OPTIONS" );
}

void GlobalOptions::load( std::vector< std::string > & output_lines ) {
  final String first_line = in.readLine();
  if( !first_line.equals( "START_OPTIONS" ) ) {
    throw new LoadFailureException( "Expected 'START_OPTIONS' instead of '" + first_line + "'" );
  }

  for( String line = in.readLine(); !line.equals( "END_OPTIONS" ); line = in.readLine() ) {
    final String[] split = line.split( "\\s+" );
    if( split.length == 0 )
      continue;

    if( split[ 0 ].equals( "show_node_titles" ) ) {
      show_node_titles_ = Bool.parseBool( split[ 1 ] );
      continue;
    }

    if( split[ 0 ].equals( "put_node_titles_to_side" ) ) {
      put_node_titles_to_side_ = Bool.parseBool( split[ 1 ] );
      continue;
    }

    if( split[ 0 ].equals( "num_processors" ) ) {
      num_processors_ = Integer.parseInt( split[ 1 ] );
      continue;
    }

    if( split[ 0 ].equals( "grid_size" ) ) {
      grid_size_ = Integer.parseInt( split[ 1 ] );
      continue;
    }

    if( split[ 0 ].equals( "default_run_command" ) ) {
      default_run_command_ = split[ 1 ];
      for( int i = 2; i < split.length; ++i ) {
	default_run_command_ += " " + split[ i ];
      }
      continue;
    }

    if( split[ 0 ].equals( "delete_unused_intermediate_poses" ) ) {
      delete_unused_intermediate_poses_ = Bool.parseBool( split[ 1 ] );
      continue;
    }

    if( split[ 0 ].equals( "serialize_intermediate_poses" ) ) {
      serialize_intermediate_poses_ = Bool.parseBool( split[ 1 ] );
      continue;
    }
  }
}


};
