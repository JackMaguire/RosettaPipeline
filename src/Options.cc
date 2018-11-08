#include <Options.hh>
#include <cassert>

Options::~Options(){}

std::string
bool_2_string( bool b ){
  return ( b ? "1" : "0" );
}

bool
string_2_bool( std::string const & s ){
  return s == "1";
}

////////////
// Save/Load
void Options::save( serialization::Archiver & archiver ) const {
  archiver.add_element( "START", "OPTIONS" );

  archiver.add_element( "show_node_titles" , bool_2_string( show_node_titles ) );
  archiver.add_element( "put_node_titles_to_side" , bool_2_string( put_node_titles_to_side ) );
  archiver.add_element( "grid_size" , std::to_string( grid_size ) );

  archiver.add_element( "serialize_intermediate_poses" , bool_2_string( serialize_intermediate_poses ) );
  archiver.add_element( "num_processors" , std::to_string( num_processors ) );
  archiver.add_element( "default_run_command" , default_run_command );
  archiver.add_element( "delete_unused_intermediate_poses" , bool_2_string( delete_unused_intermediate_poses ) );
  archiver.add_element( "END", "OPTIONS" );
}

void
Options::load( serialization::Unarchiver & unarchiver ) {

  {
    serialization::ArchiveElement first_element = unarchiver.get_next_element();
    assert( first_element.token == "START" );
    assert( first_element.value == "OPTIONS" );
  }

  for( serialization::ArchiveElement element = unarchiver.get_next_element();
       element.token != "END" || element.value != "OPTIONS";
       element = unarchiver.get_next_element() ){

    if( element.token == "show_node_titles" ){
      show_node_titles = string_2_bool( element.value );
      continue;
    }

    if( element.token == "put_node_titles_to_side" ){
      put_node_titles_to_side = string_2_bool( element.value );
      continue;
    }

    if( element.token == "grid_size" ){
      grid_size = std::stoi( element.value );
      continue;
    }

    if( element.token == "serialize_intermediate_poses" ){
      serialize_intermediate_poses = string_2_bool( element.value );
      continue;
    }

    if( element.token == "num_processors" ){
      num_processors = std::stoi( element.value );
      continue;
    }

    if( element.token == "default_run_command" ){
      default_run_command = element.value;
      continue;
    }

    if( element.token == "delete_unused_intermediate_poses" ){
      delete_unused_intermediate_poses = string_2_bool( element.value );
      continue;
    }

    assert( false );//TODO remove this for future-proofing?
  }

  }//while not "END" "OPTIONS"

}

