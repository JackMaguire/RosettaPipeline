#include <graph/Edge.hh>
#include <graph/Node.hh>

#include <iostream>

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


Edge::Edge(
  std::vector< NodeSP > const & nodes,
  std::vector< std::string > const & lines,
  int line_to_start_at
) {
  init();

  int current_line = line_to_start_at;

  assert( lines[ current_line ] == "START_EDGE" );
 
  while( lines[ ++current_line ] != "END_EDGE" ){
    std::string const line = lines[ current_line ];

    if( line == "START_NOTES" ) {
      while( lines[ ++current_line ] != "END_NOTES" ){
	//std::move this?
	notes_ += lines[ current_line ] + "\n";
      }
      continue;
    }//Notes

    std::vector< std::string > tokens;
    {//stolen from https://stackoverflow.com/questions/13172158/c-split-string-by-line
      std::string const delimiter = " ";
      std::string::size_type prev = 0;
      std::string::size_type pos = line.find( delimiter, prev );
      while ( pos != std::string::npos ) {
	tokens.push_back( line.substr( prev, pos - prev ) );
	prev = pos + 1;
	pos = line.find( delimiter, prev );
      }

      // To get the last substring (or only, if delimiter is not found)
      tokens.push_back( line.substr( prev ) );
    }

    if( tokens.size() < 2 ) continue;
    
    if( tokens[ 0 ] == "source" ) {
      int const node_id = std::stoi( tokens[ 1 ] );
      std::cout << "looking for node with id " << node_id << std::endl;
      bool found_a_match = false;
      for( NodeSP const & n : nodes ) {
	if( n->ID() == node_id ) {
	  std::cout << "\t" << n->ID() << std::endl;
	  source_node_ = n;
	  n->addDownstreamEdge( shared_from_this() );
	  found_a_match = true;
	  break;
	}
      }
      assert( found_a_match );
      continue;
    }//if source

    if( tokens[ 0 ] == "destination" ) {
      int const node_id = std::stoi( tokens[ 1 ] );
      std::cout << "looking for node with id " << node_id << std::endl;
      bool found_a_match = false;
      for( NodeSP const & n : nodes ) {
	std::cout << "\t" << n->ID() << std::endl;
	if( n->ID() == node_id ) {
	  destination_node_ = n;
	  n->addUpstreamEdge( shared_from_this() );
	  found_a_match = true;
	  break;
	}
      }
      assert( found_a_match );
      continue;
    }//if destination

    if( tokens[ 0 ] == "column" ) {
      column_name_to_sort_by_ = tokens[ 1 ];
      continue;
    }

    if( tokens[ 0 ] == "pos_is_better" ) {
      positive_scores_are_better_ = ( tokens[ 1 ] == "1" );
      continue;
    }

    if( tokens[ 0 ] == "num" ) {
      num_results_to_transfer_ = std::stoi( tokens[ 1 ] );
      continue;
    }

    if( tokens[ 0 ] == "perc" ) {
      percentage_of_results_to_transfer_ = std::stod( tokens[ 1 ] );
      continue;
    }

    if( tokens[ 0 ] == "use_perc" ) {
      use_percentage_instead_of_count_ = ( tokens[ 1 ] == "1" );
      continue;
    }

    if( current_line == lines.size() - 1 ){
      //TODO
      //throw something
    }

  }//while not END_EDGE

}

} //namespace graph
