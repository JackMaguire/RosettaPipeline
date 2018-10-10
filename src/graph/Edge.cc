#include <graph/Edge.hh>
#include <graph/Node.hh>

namespace graph {

void Edge::init(){
  column_name_to_sort_by_ = "total_score";
  positive_scores_are_better_ = false;

  num_results_to_transfer_ = 0;
  percentage_of_results_to_transfer_ = 50;
  use_percentage_instead_of_count_ = false;

  notes_ = "";
}

}
