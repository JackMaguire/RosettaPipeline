#pragma once

#include <memory>

namespace graph {

class Node;
using NodeSP = std::shared_ptr< Node >;
using NodeCSP = std::shared_ptr< const Node >;
using NodeWP = std::weak_ptr< Node >;
using NodeCWP = std::weak_ptr< const Node >;


}//namespace graph
