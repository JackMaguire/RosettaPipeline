#pragma once

//Don't look, macros!!!
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define FILE_STRING STRINGIZE(__FILE__)

#include <iostream>
#include <time.h>
#include <string>

namespace util {

std::string
getPathToTopDirectory(){
  std::string path_to_this_file( STRINGIZE( ROSETTA_PIPELINE_PWD ) );
  return path_to_this_file;
}

std::string
generate_random_string( int size ){
  srand( time(NULL) );

  std::string chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

  std::string out = "";
  for (int i = 0; i < size; ++i) {
    out += chars[ rand() % chars.size() ];
  }
  return out;
}


}
