#pragma once

#include <serialization.hh>
#include <Options.hh>
#include <graph/Graph.fwd.hh>

namespace serialization {

std::string save(
  Options const &,
  graph::Graph const &
);




}
