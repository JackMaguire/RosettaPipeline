#pragma once

#include <string>
#include <vector>

namespace publishing {

struct Publication {
  std::string title;
  std::string author;
  std::string tags;
  std::string key;
  bool is_private;
  std::string filepath;
};

std::vector< Publication >
load( std::string const & directory );

}
