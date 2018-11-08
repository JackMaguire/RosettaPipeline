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

void Edge::save( serialization::Archiver & archiver ) const {
  archiver.add_element( "START", "EDGE" );

  archiver.add_element( "source", std::to_string( source_node_.lock()->ID() ) );
  archiver.add_element( "destination", std::to_string( destination_node_.lock()->ID() ) );
  archiver.add_element( "column", column_name_to_sort_by_ );
  archiver.add_element( "pos_is_better", ( positive_scores_are_better_ ? "1" : "0" ) );

  archiver.add_element( "num", std::to_string( num_results_to_transfer_ ) );
  archiver.add_element( "frac", std::to_string( fraction_of_results_to_transfer_ ) );
  archiver.add_element( "use_frac", ( use_fraction_instead_of_count_ ? "1" : "0" ) );

  archiver.add_element( "notes", notes_ );

  archiver.add_element( "END", "EDGE" );
}

void
Edge::load(
  std::vector< NodeSP > const & nodes,
  serialization::Unarchiver & unarchiver
) {
  //START line has already been checked by Graph

  for( serialization::ArchiveElement element = unarchiver.get_next_element();
       element.token != "END" || element.value != "EDGE";
       element = unarchiver.get_next_element() ){

    if( element.token == "column" ){
      column_name_to_sort_by_ = element.value;
      continue;
    }

    if( element.token == "pos_is_better" ){
      positive_scores_are_better_ = ( element.value == "1" );
      continue;
    }

    if( element.token == "num" ){
      num_results_to_transfer_ = std::stoi( element.value );
      continue;
    }

    if( element.token == "frac" ){
      fraction_of_results_to_transfer_ = std::stod( element.value );
      continue;
    }

    if( element.token == "use_frac" ){
      use_fraction_instead_of_count_ = ( element.value == "1" );
      continue;
    }

    if( element.token == "notes" ){
      notes_ = element.value;
      continue;
    }

    if( element.token == "source" ){
      int const node_id = std::stoi( element.value );
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
    }

    if( element.token == "destination" ){
      int const node_id = std::stoi( element.value );
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
    }

  }
 
  while( lines[ ++current_line ] != "END_EDGE" ){
    std::string const line = lines[ current_line ];

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

  }//while not END_EDGE

}

} //namespace graph
