#pragma once

//Don't look, macros!!!
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define FILE_STRING STRINGIZE(__FILE__)

#include <iostream>
#include <time.h>
#include <string>

namespace util {

inline
std::string
getPathToTopDirectory(){
  std::string path_to_this_file( STRINGIZE( ROSETTA_PIPELINE_PWD ) );
  return path_to_this_file;
}

inline
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

//Stolen from https://stackoverflow.com/questions/874134/find-if-string-ends-with-another-string-in-c
inline
bool
hasEnding ( std::string const & fullString, std::string const & ending ) {
  if( fullString.length() < ending.length() ) {
    return false;
  }

  return fullString.compare ( fullString.length() - ending.length(), ending.length(), ending ) == 0;
}


}//util
