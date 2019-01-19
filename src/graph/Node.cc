#include <graph/Node.hh>
#include <graph/Edge.hh>

#include <iostream>

#include <sstream>

namespace graph {

namespace {
//Stolen from https://stackoverflow.com/questions/874134/find-if-string-ends-with-another-string-in-c
bool hasEnding ( std::string const & fullString, std::string const & ending ) {
  if( fullString.length() < ending.length() ) {
    return false;
  }

  return fullString.compare ( fullString.length() - ending.length(), ending.length(), ending ) == 0;
}
}

Node::Node( Options const & options, int x, int y, Graph * parent ) :
  x_( x ),
  y_( y ),
  title_( "Node_0" ),
  parent_( parent )
{
  init( options );
}

Node::Node(
  Options const & options,
  std::string title, //pass-by-value string is on purpose
  int x,
  int y,
  Graph * parent
) :
  x_( x ),
  y_( y ),
  title_( std::move( title ) ),
  parent_( parent )
{
  init( options );
}

Node::~Node(){}

void Node::init( Options const & options ){
  assert( hasEnding( uniqueToken(), "_NODE" ) );

  use_default_command_ = true;
  id_ = 0;

  use_script_file_ = false;
  xml_script_filename_ = "script.xml";
  xml_script_ =
    "<ROSETTASCRIPTS>\n"
    "    <SCOREFXNS>\n"
    "    </SCOREFXNS>\n"
    "    <RESIDUE_SELECTORS>\n"
    "    </RESIDUE_SELECTORS>\n"
    "    <TASKOPERATIONS>\n"
    "    </TASKOPERATIONS>\n"
    "    <FILTERS>\n"
    "    </FILTERS>\n"
    "    <MOVERS>\n"
    "    </MOVERS>\n"
    "    <PROTOCOLS>\n"
    "    </PROTOCOLS>\n"
    "    <OUTPUT />\n"
    "</ROSETTASCRIPTS>\n";

  stage_ = 0;
  stage_is_valid_ = false;

  command_ = options.default_run_command;
  notes_ = "";

  user_rosetta_flags_ = "# Keep in mind that all commands will be run one directory deeper.\n"
    "# You would need to pass '-s ../pose.pdb' instead of '-s pose.pdb'\n"
    "\n# -nstruct 1\n";

  //TODO
  //command_ = Options.getDefaultRunCommand();
}

int
Node::inDegreeIgnoringTheseNodes( std::vector< NodeCSP > const & nodes_to_ignore ) const {
  int degree = 0;
  for( EdgeSP const & e : upstream_edges_ ) {
    NodeSP const upstream_node = e->getSourceNodeSP();
    auto const iter = std::find( nodes_to_ignore.begin(), nodes_to_ignore.end(), upstream_node );
    if( iter == nodes_to_ignore.end() ) {
      ++degree;
    }
  }
  return degree;
}

std::string
Node::dirname() const {
  std::string title = title_;
  for ( std::string::iterator it = title.begin(); it != title.end(); ++it ) {
    if ( *it == ' ' || *it == '\t' ) {
      *it = '_';
    }
  }
  return "stage" + std::to_string( stage() ) + "_" + title;
}

std::string
Node::determineAutoFlags() const {
  std::stringstream ss;

  if( upstream_edges_.size() > 0 ) {
    ss <<  "-l input_files\n";
  } else {
    ss << "# Don't forget to add input file flags (-s, -l, etc)\n";
  }

  if( downstream_edges_.size() > 0 ) {
    // vec.emplace_back( "# -out:file:srlz 1 # Activate this by checking the
    // 'Serialize
    // Intermediate Poses' box in the compile tab" );
  }

  if( use_script_file_ ) {
    ss << "-parser:protocol ../" << xml_script_filename_ << "n";
    } else {
    ss << "-parser:protocol script.xml\n";
  }

  return ss.str();
}

std::string
Node::commonFlags() {
  std::stringstream ss;
  ss << "-linmem_ig 10                   # save time and memory during packing\n";
  ss << "-ignore_unrecognized_res true   # false by default\n";
  ss << "-ignore_waters false            # true by default\n";
  ss << "-mpi_tracer_to_file mpi_\n";
  return ss.str();
}

std::string
Node::getAllRosettaFlags() const {
  return determineAutoFlags() + "\n" + user_rosetta_flags_;
}

void Node::save( serialization::Archiver & archiver ) const {
  archiver.add_element( "START", uniqueToken() );

  archiver.add_element( "id",  std::to_string( id_ ) );
  archiver.add_element( "x",  std::to_string( x_ ) );
  archiver.add_element( "y",  std::to_string( y_ ) );
  archiver.add_element( "command",  command_ );
  archiver.add_element( "title",  title_ );
  archiver.add_element( "xml_script_filename", xml_script_filename_ );
  archiver.add_element( "use_script_file", ( use_script_file_ ? "1" : "0" ) );
  archiver.add_element( "use_default_command", ( use_default_command_ ? "1" : "0" ) );
  archiver.add_element( "still_using_default_title", ( still_using_default_title_ ? "1" : "0" ) );

  archiver.add_element( "flags", user_rosetta_flags_ );
  archiver.add_element( "notes", notes_ );
  archiver.add_element( "xml_script", xml_script_ );

  archiver.add_element( "END", "NODE" );
}



Node::Node(
  serialization::Unarchiver & unarchiver,
  Options const & options,
  Graph * parent
) :
  parent_( parent )
{
  init( options );

  //START line has already been checked by Graph

  for( serialization::ArchiveElement element = unarchiver.get_next_element();
       element.token != "END" || element.value != "NODE";
       element = unarchiver.get_next_element() ){

    if( element.token == "id" ){
      id_ = std::stoi( element.value );
      continue;
    }

    if( element.token == "x" ){
      x_ = std::stoi( element.value );
      continue;
    }

    if( element.token == "y" ){
      y_ = std::stoi( element.value );
      continue;
    }

    if( element.token == "command" ){
      command_ = element.value;
      continue;
    }

    if( element.token == "title" ){
      title_ = element.value;
      continue;
    }

    if( element.token == "xml_script_filename" ){
      xml_script_filename_ = element.value;
      continue;
    }

    if( element.token == "use_script_file" ){
      use_script_file_ = (element.value == "1");
      continue;
    }

    if( element.token == "use_default_command" ){
      use_default_command_ = (element.value == "1");
      continue;
    }

    if( element.token == "still_using_default_title" ){
      still_using_default_title_ = (element.value == "1");
      continue;
    }

    if( element.token == "flags" ){
      user_rosetta_flags_ = element.value;
      continue;
    }

    if( element.token == "notes" ){
      notes_ = element.value;
      continue;
    }

    if( element.token == "xml_script" ){
      xml_script_ = element.value;
      continue;
    }

    // Call derived class
    load_from_token( element.token, element.value );

  }


}//load ctor

void
Node::addToRunScript( Options const & options, std::stringstream & run_script ) const {
  run_script << "#!/bin/bash\n";
  run_script << "nproc=" << options.num_processors << "\n";
  run_script << getEffectiveCommand( options ) << " || exit 1\n\n";
  run_script << "grep -v 'SEQUENCE:' score.sc > _data.txt\n";
}


}//namespace graph
