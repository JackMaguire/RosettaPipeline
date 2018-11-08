#pragma once

#include <string>
#include <Options.fwd.hh>
#include <graph/Graph.fwd.hh>

namespace serialization {

//Structs and classes
struct ArchiveElement {
  ArchiveElement(
    std::string const & t,
    std::string const & v
  ):
    token( t ),
    value( v )
  {}

  std::string token;
  std::string value;
};

class Archiver {
public:
  virtual void add_element( std::string const & token, std::string const & value ) = 0;
};

class Unarchiver {
public:
  virtual ArchiveElement const & get_next_element() = 0;
};


std::string save(
  Options const &,
  graph::Graph const &
);


//returns a message, hopefully "load successful"
//possibly replace this with an enum?
std::string load_file(
  std::string const & filename,
  graph::Graph & graph,
  Options & options
);


}


