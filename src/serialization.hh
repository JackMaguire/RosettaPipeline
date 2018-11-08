#pragma once

#include <string>
#include <Options.hh>
#include <graph/Graph.fwd.hh>

namespace serialization {

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


