#pragma once

#include <memory>

namespace graph {

class GeneralNode;
using GeneralNodeSP = std::shared_ptr< GeneralNode >;
using GeneralNodeCSP = std::shared_ptr< const GeneralNode >;
using GeneralNodeWP = std::weak_ptr< GeneralNode >;
using GeneralNodeCWP = std::weak_ptr< const GeneralNode >;


}//namespace graph
