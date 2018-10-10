#pragma once

#include <graph/Node.fwd.hh>

namespace graph {

class Edge {

protected:
  void init(){
    num_results_to_transfer_ = 0;
    percentage_of_results_to_transfer_ = 50;
    use_percentage_instead_of_count_ = false;
  }

private:
  NodeSP source_node_;
  NodeSP destination_node_;

  std::string column_name_to_sort_by_;
  bool positive_scores_are_better_;

  int num_results_to_transfer_;
  double percentage_of_results_to_transfer_;
  bool use_percentage_instead_of_count_;

  std::string notes_;
};

}//namespace graph
