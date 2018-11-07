#include <publishing/load.hh>
//#include <cassert>
//#include <memory>
#include <filesystem>
#include <fstream>

namespace publishing {

std::vector< Publication >
load( std::string const & directory ){
  std::vector< Publication > all_publications;

  if( ! std::filesystem::exists( directory ) ) {
    return all_publications;
  }

  for( auto & subdir : std::filesystem::directory_iterator( directory ) ){
    if( std::filesystem::is_directory( subdir ) ){
      std::string const subdir_str( subdir.path().string() );

      std::string const info_file = subdir_str + "/info.txt";
      if( ! std::filesystem::exists( info_file ) ) continue;

      std::string save_file = subdir_str + "/save.txt";
      if( ! std::filesystem::exists( save_file ) ) continue;

      std::ifstream reader( info_file );
      if( reader.good() ){
	all_publications.emplace_back();
	Publication & new_pub = all_publications.back();
	new_pub.key = subdir_str.substr( subdir_str.rfind("/") + 1 );
	new_pub.filepath = std::move( save_file );
	new_pub.is_private = false;//This can be overriden later

	for( std::string line; std::getline( reader, line ); ) {
	  std::string const first_token = line.substr( 0, line.find(' ') );
	  std::string arg = line.substr( line.find(' ') + 1 );

	  if( first_token == "Title" ){
	    new_pub.title = std::move( arg );
	  } else if( first_token == "Author" ){
	    new_pub.author = std::move( arg );
	  } else if( first_token == "Tags" ){
	    new_pub.tags = std::move( arg );
	  } else if ( first_token == "Private?" ){
	    new_pub.is_private = ( arg == "1" );
	  }
	}
      }

    }
  }

  return all_publications;
}


}
