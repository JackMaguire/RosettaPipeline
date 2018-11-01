#pragma once

//Don't look, macros!!!
//#define STR(str) #str
//#define STRING(str) STR(str)

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define FILE_STRING STRINGIZE(__FILE__)

#include <iostream>

namespace util {

std::string
getPathToTopDirectory(){
  std::string path_to_this_file( ROSETTA_PIPELINE_PWD );
  return path_to_this_file;
}

}
