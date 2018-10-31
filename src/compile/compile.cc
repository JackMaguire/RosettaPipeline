#include <compile/compile.hh>

#include <graph/Graph.hh>
#include <graph/Node.hh>
#include <graph/Edge.hh>

#include <global_data/options.hh>
#include <global_data/version.hh>

#include <set>
#include <sstream>
//#include <stdlib.h>
#include <iostream>
//#include <stdio.h>//remove
#include <filesystem>//remove
#include <cstdlib>//system
#include <time.h>
#include <fstream>
#include <string>

using namespace graph;

namespace compile {

namespace {

std::string
generate_random_string( int size ){
  srand( time(NULL) );

  std::string chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

  std::string out = "";
  for (int i = 0; i < size; ++i) {
    out += chars[ rand() % chars.size() ];
  }
  return out;
}

}

script_pair
compile( graph::Graph const & g ){
  script_pair scripts;

  if( cycleExists( g ) ){
    scripts.setup_script = "Error: could not compile, cycle exists in graph";
    scripts.run_script = scripts.setup_script;
    return scripts;
  }

  auto const nproc = global_data::Options::num_processors;

  if( nproc == 0 ){
    scripts.setup_script = "Error: could not compile, num_processors value is not set."
      "Please go to the 'Options' tab and select a number larger than 0";
    scripts.run_script = scripts.setup_script;
    return scripts;
  }

  if( nproc < 0 ){
    scripts.setup_script = std::to_string( nproc ) +
      "is an invalid value for num_processors. Please set this to a positive number.";
    scripts.run_script = scripts.setup_script;
    return scripts;
  }


  std::vector< graph::NodeCSP > const nodes_in_order = determineOrderOfNodes( g );

  for( uint stage = 1; stage <= nodes_in_order.size(); ++stage ) {
    nodes_in_order[ stage - 1 ]->setStage( stage );
    nodes_in_order[ stage - 1 ]->setStageValidity( true );
  }

  std::string const directory_name = setup_working_directory( nodes_in_order );
  std::string const subdirectory_name = directory_name + "/rosetta_pipeline";

  {//run script
    std::string const run_script_filename = subdirectory_name + "/run.sh";
    std::ofstream run_script;
    run_script.open( run_script_filename );
    compile_run_script( nodes_in_order, run_script );
    run_script.close();
  }

  for( graph::NodeCSP const & node : nodes_in_order ) {
    node->setStageValidity( false );
  }

  std::string const tar_file = directory_name + "/rosetta_pipeline.tar.gz";
  std::string const command = "tar -czf " + tar_file + " " + directory_name + "/rosetta_pipeline";
  int const system_result = std::system( command.c_str() );
  assert( system_result == 0 );


  std::string tar_file_contents;
  {
    std::ifstream in( tar_file, std::ios::in | std::ios::binary );
    assert( in );
    std::ostringstream contents;
    contents << in.rdbuf();
    in.close();
    tar_file_contents = contents.str();
  }
 

  std::filesystem::remove_all( directory_name );
  return scripts;
}

std::string
just_compile_run_script( graph::Graph const & g ) {
  if( cycleExists( g ) ){
    return "Error: could not compile, cycle exists in graph";
  }

  auto const nproc = global_data::Options::num_processors;

  if( nproc == 0 ){
    return "Error: could not compile, num_processors value is not set."
      "Please go to the 'Options' tab and select a number larger than 0";
  }

  if( nproc < 0 ){
    return std::to_string( nproc ) +
      "is an invalid value for num_processors. Please set this to a positive number.";
  }


  std::vector< graph::NodeCSP > const nodes_in_order = determineOrderOfNodes( g );

  for( uint stage = 1; stage <= nodes_in_order.size(); ++stage ) {
    nodes_in_order[ stage - 1 ]->setStage( stage );
    nodes_in_order[ stage - 1 ]->setStageValidity( true );
  }

  std::string const run_script_filename = subdirectory_name + "/run.sh";
  std::stringstream run_script;
  compile_run_script( nodes_in_order, run_script );

  for( graph::NodeCSP const & node : nodes_in_order ) {
    node->setStageValidity( false );
  }

  return run_script.str();
}


std::string
setup_working_directory(
  std::vector< graph::NodeCSP > const & nodes_in_order
){
  std::string const directory_name = "/tmp/" + generate_random_string( 16 );
  std::filesystem::create_directory( directory_name );
  std::cout << "creating " << directory_name << std::endl;

  std::string const subdirectory_name = directory_name + "/rosetta_pipeline";
  std::filesystem::create_directory( subdirectory_name );

  for( graph::NodeCSP const & node : nodes_in_order ){
    std::string const subsubdirectory = subdirectory_name + "/" + node->dirname();
    std::filesystem::create_directory( subsubdirectory );

    {//xml script
      std::string const & xml_script = node->xmlScript();
      std::string const script_filename = subsubdirectory + "/script.xml";
      std::ofstream script_file;
      script_file.open( script_filename );
      script_file << xml_script << "\n";
      script_file.close();
    }

    {//flags
      //std::stringstream ss( node->getAllRosettaFlags() );
      std::string const all_flags = node->getAllRosettaFlags();
      std::string const flags_filename = subsubdirectory + "/flags";
      std::ofstream flags_file;
      flags_file.open( flags_filename );
      flags_file << all_flags << "\n";

      if( global_data::Options::serialize_intermediate_poses ) {
	if( node->numUpstreamEdges() > 0 ) {
	  flags_file << "-in:file:srlz_override 1\n";
	}

	if( node->numDownstreamEdges() > 0 ) {
	  flags_file << "-out:file:srlz 1\n";
	}
      }
      
      flags_file.close();
    }

    if( node->numUpstreamEdges() > 0 ) {//input files
      //create empty file
      std::string const input_filename = subsubdirectory + "/input_filename";
      std::ofstream input_file;
      input_file.open( input_filename );
      input_file << "";
      input_file.close();
    }    
  }

  return directory_name;
}

void
compile_run_script(
  std::vector< graph::NodeCSP > const & nodes_in_order,
  std::ostream & run_script
){

  addGlobalIntroToScript( run_script );
  
  for( graph::NodeCSP const & node : nodes_in_order ){
    addStageIntroToScript( node->stage(), run_script );

    std::string const dirname = node->dirname();
    run_script << "cd " << dirname << "\n";

    // THE COMMAND

    run_script << "if " << node->getEffectiveCommand() << " ;then \n"
      "    echo \"Done running " << dirname << "\" >> ../JD3BASH_runlog.txt\n"
      "else\n"
      "    echo \"Failed to run " << dirname << "\" >> ../JD3BASH_runlog.txt\n"
      "    exit 1\n"
      "fi\n";

    run_script << "grep -v 'SEQUENCE:' score.sc > no_first_line.score.sc\n";
    for( EdgeSP const & de : node->getDownstreamEdges() ) {
      std::string const name_of_next_stage_directory = de->destinationNode().dirname();
      std::string const & sort_column = de->columnNameToSortBy();
      run_script << "\n#####\n" <<
	"# Extract the best results for stage \"" <<
	de->destinationNode().title() << "\"\n" <<
	"# This awk command prints the data for the column with header " <<
	sort_column << " along with the title for each result\n" <<
	"awk -v c1=\"" << sort_column <<
	"\" 'NR==1 {for (i=1; i<=NF; i++) {ix[$i] = i}}NR>1 {print $ix[c1] \" \" $NF}' no_first_line.score.sc > temp\n";

      if( de->positiveScoresAreBetter() ) {
	//TODO add support for scientific notation?
	run_script << "sort -nrk1 temp > temp2\n";
      } else {
	run_script << "sort -nk1 temp > temp2\n";
      }

      run_script << "x=`cat no_first_line.score.sc | wc -l`\n";
      if( de->useFractionInsteadOfCount() ) {
	run_script << "perc=\"" << de->fractionOfResultsToTransfer() << "\"\n";
	run_script << "nresults=`echo \"($x - 1) * $perc / 1\" | bc`\n";
      } else {
	run_script << "nresults=\"" << de->numResultsToTransfer() << "\"\n";
      }
      run_script << "# Extract structures that will survive until the next stage\n";
      run_script << "head -n $nresults temp2 | awk '{print $2}' > temp3\n";

      run_script << "# move successful runs to next stage if not there already\n";
      run_script << "destination=../" << name_of_next_stage_directory << "/input_files\n";

      /*
       * cat temp3 | while read line; do
       *   if [[ `grep $line $destination | wc -l` -eq 0 ]]; then
       *     echo `pwd`/$line.* >> $destination
       *   fi
       * done
       */
      run_script << "cat temp3 | while read line; do\n"
	" if [[ `grep $line $destination | wc -l` -eq 0 ]]; then\n"
	"  echo `pwd`/$line.* >> $destination\n"
	" fi\n"
	"done\n";

      if( global_data::Options::delete_unused_intermediate_poses && node->numDownstreamEdges() > 0 ) {
	// Save good files so that they do not get deleted later
	run_script << "\n#Save good files so that they do not get deleted later\n";
	run_script << "cat temp3 | while read line; do echo $line.* ; done > results_to_keep.txt\n";
      }
    }

    if( global_data::Options::delete_unused_intermediate_poses && node->numDownstreamEdges() > 0 ) {
      run_script << "\n# Delete poses not needed for future stages\n" <<
	"awk '{print $2}' temp | while read line; do\n"
	"    if [[ `grep $line temp3 | wc -l` -eq 0 ]]; then\n"
	"        rm $line.*\n"
	"    fi\n"
	"done\n";
    }

    run_script << "\ncd ..\n";
    run_script << "echo \"Done With " << dirname << "\" >> JD3BASH_runlog.txt\n";
  }
}

bool cycleExists( NodeCSP const & starting_node, std::set< NodeCSP > & nodes_already_visited ) {
  if( nodes_already_visited.find( starting_node ) != nodes_already_visited.end() ) {
    return true;
  }

  auto const iter_bool_pair = nodes_already_visited.insert( starting_node );

  for( EdgeSP const & e : starting_node->getDownstreamEdges() ) {
    if( cycleExists( e->getDestinationNodeCSP(), nodes_already_visited ) ) {
      return true;
    }
  }

  nodes_already_visited.erase( iter_bool_pair.first );

  return false;
}

bool cycleExists( Graph const & g ) {
  // Recursion
  // Can use unordered_set, but this is so small it likely won't make a difference
  std::set< NodeCSP > nodes_already_visited;
  for( NodeSP const & n : g.nodes() ) {
    if( cycleExists( n, nodes_already_visited ) ) {
      return true;
    }
  }
  return false;
}


std::vector< graph::NodeCSP > determineOrderOfNodes( graph::Graph const & g ) {
  std::vector< graph::NodeSP > unassigned_nodes = g.nodes();//Copy!
  std::vector< graph::NodeCSP > assigned_nodes_in_order;

  while ( true ) {
    // Add nodes that do not depend on any unassigned node
    // Work backwards so that we can easily delete while we work
    for( int j = unassigned_nodes.size() - 1; j >= 0; --j ) {
      graph::NodeCSP const & u_node = unassigned_nodes[ j ];
      if( u_node->inDegreeIgnoringTheseNodes( assigned_nodes_in_order ) == 0 ) {
	assigned_nodes_in_order.push_back( u_node );
	unassigned_nodes.erase( unassigned_nodes.begin() + j );
      }
    }

    if( unassigned_nodes.size() == 0 ) {
      break;
    }
  }

  return assigned_nodes_in_order;
}

void addGlobalIntroToScript( std::stringstream & script ) {
  script << "# Script was created using JD3BASH\n";
  script << "# Version number: " << global_data::Version::number << "\n";
  script << "# Visit github.com/JackMaguire/JD3Bash for details\n\n";
}

void addStageIntroToScript( int stage, std::stringstream & script ) {
  script << "\n###########\n";
  script << "# STAGE " << stage << " #\n";
  script << "###########\n\n";
}

void addGlobalVariablesToRunScript( std::stringstream & script ) {
  script << "nproc=" << global_data::Options::num_processors << "\n";
}


}//namespace compile
