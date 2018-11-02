#include <graph/Edge.hh>
#include <graph/Node.hh>

#include <iostream>
#include <sstream>

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
  fraction_of_results_to_transfer_ = 0.25;
  use_fraction_instead_of_count_ = false;

  notes_ = "";
}

namespace {
//Stolen from https://stackoverflow.com/questions/874134/find-if-string-ends-with-another-string-in-c
bool hasEnding ( std::string const & fullString, std::string const & ending ) {
  if( fullString.length() < ending.length() ) {
    return false;
  }

  return fullString.compare ( fullString.length() - ending.length(), ending.length(), ending ) == 0;
}
}

void Edge::save( std::vector< std::string > & output_lines ) const {

  output_lines.emplace_back( "START_EDGE" );

  output_lines.emplace_back( "source " + std::to_string( source_node_.lock()->ID() ) );
  output_lines.emplace_back( "destination " + std::to_string( destination_node_.lock()->ID() ) );
  output_lines.emplace_back( "column " + column_name_to_sort_by_ );
  output_lines.emplace_back( std::string( "pos_is_better " ) +
    ( positive_scores_are_better_ ? "1" : "0" ) );

  output_lines.emplace_back( "num " + std::to_string( num_results_to_transfer_ ) );
  output_lines.emplace_back( "frac " + std::to_string( fraction_of_results_to_transfer_ ) );
  output_lines.emplace_back( std::string( "use_frac " ) +
    ( use_fraction_instead_of_count_ ? "1" : "0" ) );

  output_lines.emplace_back( "START_NOTES" );
  if( hasEnding( notes_, "\n") ){
    output_lines.emplace_back( notes_ );
  } else {
    output_lines.emplace_back( notes_ + "\n" );
  }
  output_lines.emplace_back( "END_NOTES" );

  output_lines.emplace_back( "END_EDGE" );
}

void
Edge::load(
  std::vector< NodeSP > const & nodes,
  std::vector< std::string > const & lines,
  int line_to_start_at
) {
  unsigned int current_line = line_to_start_at;

  assert( lines[ current_line ] == "START_EDGE" );
 
  while( lines[ ++current_line ] != "END_EDGE" ){
    std::string const line = lines[ current_line ];

    if( line == "START_NOTES" ) {
      std::stringstream ss;
      while( lines[ ++current_line ] != "END_NOTES" ){
	ss << std::move( lines[ current_line ] ) << "\n";
      }
      notes_ = ss.str();
      continue;
    }

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
      bool found_a_match = false;
      for( NodeSP const & n : nodes ) {
	if( n->ID() == node_id ) {
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
      bool found_a_match = false;
      for( NodeSP const & n : nodes ) {
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

    if( tokens[ 0 ] == "frac" ) {
      fraction_of_results_to_transfer_ = std::stod( tokens[ 1 ] );
      continue;
    }

    if( tokens[ 0 ] == "use_frac" ) {
      use_fraction_instead_of_count_ = ( tokens[ 1 ] == "1" );
      continue;
    }

    if( current_line == lines.size() - 1 ){
      //TODO
      //throw something
    }

  }//while not END_EDGE

}

} //namespace graph
