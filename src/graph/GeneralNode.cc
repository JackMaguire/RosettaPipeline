#include <graph/GeneralNode.hh>
#include <graph/Edge.hh>

#include <iostream>

#include <sstream>

namespace graph {

namespace {


}

GeneralNode::GeneralNode( Options const & options, int x, int y, Graph * parent ) :
  Node( options, x, y, parent ),

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

}

}//namespace graph
