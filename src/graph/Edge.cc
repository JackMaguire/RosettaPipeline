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

}

void
Edge::addToRunScript( std::stringstream & run_script ) const {
  std::string const name_of_next_stage_directory = destinationNode().dirname();
  std::string const & sort_column = columnNameToSortBy();
  run_script << "\n\n#####\n" <<
    "# Extract the best results for stage \"" <<
    destinationNode().title() << "\"\n" <<
    "# This awk command prints the data for the column " <<
    sort_column << " along with the title for each result and saves it to _temp\n" <<
    "# The header line is excluded\n" <<
    "awk -v c1=\"" << sort_column <<
    "\" 'NR==1 {for (i=1; i<=NF; i++) {ix[$i] = i}}NR>1 {print $ix[c1] \" \" $NF}' _data.txt > _temp\n";

  run_script << "\n# Sort results\n";
  if( positiveScoresAreBetter() ) {
    run_script << "# positive scores are better\n";
    //TODO add support for scientific notation?
    run_script << "sort -nrk1 _temp > _temp2\n";
  } else {
    run_script << "# negative scores are better\n";
    run_script << "sort -nk1 _temp > _temp2\n";
  }

  run_script << "\n# _temp (unsorted) and _temp2 (sorted) are both in the format \"[metric] [unique file prefix]\"\n";
  run_script << "# _temp3 (sorted and truncated) is in the format \"[unique file prefix]\"\n\n";

  if( useFractionInsteadOfCount() ) {
    run_script << "num_results=`cat _temp2 | wc -l`\n";
    run_script << "frac=\"" << fractionOfResultsToTransfer() << "\"\n";
    run_script << "num_results_to_keep_float=`echo \"($num_results) * $frac\" | bc`\n";
    run_script << "num_results_to_keep=${num_results_to_keep_float%.*} #convert to integer\n";
  } else {
    run_script << "num_results_to_keep=\"" << numResultsToTransfer() << "\"\n";
  }

  run_script << "\n";

  run_script << "# Extract structures that will survive until the next stage\n";
  run_script << "head -n $num_results_to_keep _temp2 | awk '{print $2}' > _temp3\n";

  run_script << "\n";

  run_script << "# move successful runs to next stage if not there already\n";
  run_script << "destination=../" << name_of_next_stage_directory << "/input_files\n";

  run_script << "while read line; do\n"
    "    # Each $line is a (hopefully) unique identifier to a structure\n"
    "    if [[ ! `grep $line $destination` ]]; then\n"
    "        ls `pwd`/${line}* >> $destination\n"
    //"        echo $line.* >> _results_to_keep.txt\n"
    "    fi\n"
    "done << _temp3\n";

  //run_script << "\n# Save good files so that they do not get deleted later\n";
  //run_script << "cat _temp3 | while read line; do echo $line.* ; done >> _results_to_keep.txt\n\n";

}

} //namespace graph
