#pragma once

#include <memory>

namespace graph {

class Edge;
using EdgeSP = std::shared_ptr< Edge >;
using EdgeCSP = std::shared_ptr< const Edge >;

}//namespace graph
