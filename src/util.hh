#pragma once

//Don't look, macros!!!
//#define STR(str) #str
//#define STRING(str) STR(str)

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define FILE_STRING STRINGIZE(__FILE__)

namespace util {

std::string
getPathToTopDirectory(){
  //std::string path_to_this_file = STRING( __FILE__ );//Users/.../RosettaPipeline/src/util.hh
  std::string path_to_this_file( FILE_STRING );//Users/.../RosettaPipeline/src/util.hh
  std::string const toErase = "src/util.hh";
  size_t pos = path_to_this_file.find( toErase );
  assert( pos != std::string::npos );
  path_to_this_file.erase( pos, toErase.length() );
  return path_to_this_file;
}

}
