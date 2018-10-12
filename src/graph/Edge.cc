#include <graph/Edge.hh>
#include <graph/Node.hh>

namespace graph {

Edge::Edge() :
  source_node_(),
  destination_node_()
{
  init();
}

Edge::Edge( NodeSP const & source_node, NodeSP const & destination_node ) :
  source_node_( source_node ),
  destination_node_( destination_node )
{
  init();
}

Edge::~Edge(){}


void Edge::init(){
  column_name_to_sort_by_ = "total_score";
  positive_scores_are_better_ = false;

  num_results_to_transfer_ = 0;
  percentage_of_results_to_transfer_ = 50;
  use_percentage_instead_of_count_ = false;

  notes_ = "";
}


void Edge::save( std::vector< std::string > & output_lines ) const {

  output_lines.emplace_back( "START_EDGE" );

  output_lines.emplace_back( "source " + std::to_string( source_node_.lock()->ID() ) );
  output_lines.emplace_back( "destination " + std::to_string( destination_node_.lock()->ID() ) );
  output_lines.emplace_back( "column " + column_name_to_sort_by_ );
  output_lines.emplace_back( std::string( "pos_is_better " ) +
    ( positive_scores_are_better_ ? "1" : "0" ) );

  output_lines.emplace_back( "num " + std::to_string( num_results_to_transfer_ ) );
  output_lines.emplace_back( "perc " + std::to_string( percentage_of_results_to_transfer_ ) );
  output_lines.emplace_back( std::string( "use_perc " ) +
    ( use_percentage_instead_of_count_ ? "1" : "0" ) );

  output_lines.emplace_back( "START_NOTES" );
  {//stolen from https://stackoverflow.com/questions/13172158/c-split-string-by-line
    std::string const delimiter = "\n";
    std::string::size_type prev = 0;
    std::string::size_type pos = notes_.find( delimiter, prev );
    while ( pos != std::string::npos ) {
      output_lines.push_back( notes_.substr( prev, pos - prev ) );
      prev = pos + 1;
      pos = notes_.find( delimiter, prev );
    }

    // To get the last substring (or only, if delimiter is not found)
    output_lines.push_back( notes_.substr( prev ) );
  }
  output_lines.emplace_back( "END_NOTES" );

  output_lines.emplace_back( "END_EDGE" );

}


} //namespace graph
