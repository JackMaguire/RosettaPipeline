#pragma once

#include <string>

namespace global_data {

struct Version {

  static std::string number;

};

std::string Version::number = "0.0.0";

} //namespace
