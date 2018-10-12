#pragma once

#include <memory>

namespace graph {

class Graph;
using GraphSP = std::shared_ptr< Graph >;
using GraphCSP = std::shared_ptr< const Graph >;
using GraphWP = std::weak_ptr< Graph >;
using GraphCWP = std::weak_ptr< const Graph >;

struct PreliminaryEdge;
using PreliminaryEdgeSP = std::shared_ptr< PreliminaryEdge >;
using PreliminaryEdgeCSP = std::shared_ptr< const PreliminaryEdge >;
using PreliminaryEdgeWP = std::weak_ptr< PreliminaryEdge >;
using PreliminaryEdgeCWP = std::weak_ptr< const PreliminaryEdge >;

}//namespace graph
