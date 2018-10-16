#include <global_data/options.hh>

namespace global_data {

////////////
// Save/Load
void Options::save( std::vector< std::string > & output_lines ) {
  output_lines.emplace_back( "START_OPTIONS" );

  output_lines.emplace_back( "show_node_titles " + std::to_string( show_node_titles ) );
  output_lines.emplace_back( "put_node_titles_to_side " + std::to_string( put_node_titles_to_side ) );
  output_lines.emplace_back( "grid_size " + std::to_string( grid_size ) );

  output_lines.emplace_back( "serialize_intermediate_poses " + std::to_string( serialize_intermediate_poses ) );
  output_lines.emplace_back( "num_processors " + std::to_string( num_processors ) );
  output_lines.emplace_back( "default_run_command " + default_run_command );
  output_lines.emplace_back( "delete_unused_intermediate_poses " + std::to_string( delete_unused_intermediate_poses ) );
  output_lines.emplace_back( "END_OPTIONS" );
}

void Options::load( std::vector< std::string > const & lines, int line_to_start_at ) {

  unsigned int current_line = line_to_start_at;

  assert( lines[ current_line ] == "START_NODE" );

  while( lines[ ++current_line ] != "END_NODE" ){
    std::string const line = lines[ current_line ];

    std::vector< std::string > tokens;
    {//stolen from https://stackoverflow.com/questions/13172158/c-split-string-by-line
      std::string const delimiter = " ";
      std::string::size_type prev = 0;
      std::string::size_type pos = line.find( delimiter, prev );
      while ( pos != std::string::npos ) {
	tokens.push_back( line.substr( prev, pos - prev ) );
	prev = pos + 1;
	pos = line.find( delimiter, prev );
      }

      // To get the last substring (or only, if delimiter is not found)
      tokens.push_back( line.substr( prev ) );
    }

    if( tokens.size() < 2 ) continue;

    if( tokens[ 0 ] == "show_node_titles" ) {
      show_node_titles = ( tokens[ 1 ] == "1" );
      continue;
    }

    if( tokens[ 0 ] == "put_node_titles_to_side" ) {
      put_node_titles_to_side = ( tokens[ 1 ] == "1" );
      continue;
    }

    if( tokens[ 0 ] == "grid_size" ) {
      num_processors = std::stoi( tokens[ 1 ] );
      continue;
    }


    if( tokens[ 0 ] == "serialize_intermediate_poses" ) {
      serialize_intermediate_poses = ( tokens[ 1 ] == "1" );
      continue;
    }

    if( tokens[ 0 ] == "num_processors" ) {
      num_processors = std::stoi( tokens[ 1 ] );
      continue;
    }

    if( tokens[ 0 ] == "title" ) {
      default_run_command = tokens[ 1 ];
      for( unsigned int i = 2; i < tokens.size(); ++i ) {
	default_run_command += " " + tokens[ i ];
      }
      continue;
    }

    if( tokens[ 0 ] == "delete_unused_intermediate_poses" ) {
      delete_unused_intermediate_poses = ( tokens[ 1 ] == "1" );
      continue;
    }

  }//while not "END_NODE"
}


};
