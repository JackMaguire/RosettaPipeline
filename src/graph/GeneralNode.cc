#include <graph/GeneralNode.hh>
#include <graph/Edge.hh>

#include <iostream>

#include <sstream>

namespace graph {

namespace {

std::string get_starting_bash_script(){
  return
    "# TODO write any bash command you want here!\n"
    "# Rules:\n"
    "# 1) Don't include the '#!/bin/bash' line. We will take care of that\n"
    "# 2) If this stage has upstream nodes, there will be a file called 'input_poses' that will list absolute paths to all of the structures.\n"
    "# 3) You must store your results in a whitespace-delimited file named '_data.txt' with a header row, similar to Rosetta's score.sc file.\n"
    "#    The final column of _data.txt needs to contain a unique prefix to the corresponding output structure created by this stage (output structures MUST be in this directory).\n"
    "# Here is an example of a stage that counts the number of atoms in each pose:\n"

    "echo num_atoms num_ARG_atoms file > _data.txt\n"
    "count=0\n"
    "while read path_to_struct; do\n"
    "    name=$(basename $path_to_struct)\n"
    "    count=$((count+1))\n"
    "    local_filename=$count.$name\n"
    "    cp $path_to_struct $local_filename\n"
    "    num_atoms=$(grep ATOM $local_filename | wc -l)\n"
    "    num_arginine_atoms=$(grep ATOM $local_filename | grep -i arg | wc -l)\n"
    "    echo $num_atoms $num_arginine_atoms $local_filename >> _data.txt\n"
    "done; < input_poses\n"
    ;
}

}

GeneralNode::GeneralNode( Options const & options, int x, int y, Graph * parent ) :
  Node( options, x, y, parent ),
  bash_script_( get_starting_bash_script() )
{}

GeneralNode::GeneralNode(
  Options const & options,
  std::string const & title,
  int x,
  int y,
  Graph * parent
) :
  Node( options, title, x, y, parent ),
  bash_script_( get_starting_bash_script() )
{}

GeneralNode::~GeneralNode(){}

void GeneralNode::save_additional_tokens( serialization::Archiver & archiver ) const {
  archiver.add_element( "general_bash_script", bash_script_ );
}

void
GeneralNode::load_from_token(
  std::string const & token,
  std::string const & value
){
  if( token == "general_bash_script" ){
    bash_script_ = value;
  }
}


GeneralNode::GeneralNode(
  serialization::Unarchiver & unarchiver,
  Options const & options,
  Graph * parent
) :
  Node( unarchiver, options, parent )
{}//load ctor


void
GeneralNode::addToRunScript(
  Options const &,
  std::stringstream & run_script
) const {
  //run_script << "#!/bin/bash\n";
  run_script << bash_script_ << "\n";
}

}//namespace graph
