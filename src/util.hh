#pragma once

//Don't look, macros!!!
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define FILE_STRING STRINGIZE(__FILE__)

#include <iostream>

namespace util {

std::string
getPathToTopDirectory(){
  std::string path_to_this_file( STRINGIZE( ROSETTA_PIPELINE_PWD ) );
  return path_to_this_file;
}

}
