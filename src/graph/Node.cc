#include <graph/Node.hh>
#include <graph/Edge.hh>

#include <global_data/options.hh>

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

Node::Node( int x, int y ) {
  title_ = "Node_0";
  x_ = x;
  y_ = y;
  init();
}

Node::Node( std::string title, int x, int y ) {//pass-by-value on purpose
  title_ = std::move( title );
  x_ = x;
  y_ = y;
  init();
}

Node::~Node(){}

void Node::init(){
  assert( hasEnding( uniqueToken(), "_NODE" ) );

  use_default_command_ = true;

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

  command_ = global_data::Options::default_run_command;
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

void Node::save( std::vector< std::string > & output_lines ) const {
  output_lines.emplace_back( "START_" + uniqueToken() );
  output_lines.emplace_back( "id " + std::to_string( id_ ) );
  output_lines.emplace_back( "x " + std::to_string( x_ ) );
  output_lines.emplace_back( "y " + std::to_string( y_ ) );
  output_lines.emplace_back( "r " + std::to_string( red_ ) );
  output_lines.emplace_back( "g " + std::to_string( green_ ) );
  output_lines.emplace_back( "b " + std::to_string( blue_ ) );
  output_lines.emplace_back( "command " + command_ );
  output_lines.emplace_back( "title " + title_ );
  output_lines.emplace_back( "script " + xml_script_filename_ );
  output_lines.emplace_back( std::string( "use_script_file " ) + ( use_script_file_ ? "1" : "0" ) );
  output_lines.emplace_back( std::string( "use_default_command " ) + ( use_default_command_ ? "1" : "0" ) );
  output_lines.emplace_back( std::string( "still_using_default_title " ) + ( still_using_default_title_ ? "1" : "0" ) );

  output_lines.emplace_back( "START_FLAGS" );
  output_lines.emplace_back( user_rosetta_flags_ );
  output_lines.emplace_back( "END_FLAGS" );

  output_lines.emplace_back( "START_NOTES" );
  if( hasEnding( notes_, "\n") ){
    output_lines.emplace_back( notes_ );
  } else {
    output_lines.emplace_back( notes_ + "\n" );
  }
  output_lines.emplace_back( "END_NOTES" );

  output_lines.emplace_back( "START_SCRIPT" );
  if( hasEnding( xml_script_, "\n") ){
    output_lines.emplace_back( xml_script_ );
  } else {
    output_lines.emplace_back( xml_script_ + "\n" );
  }
  output_lines.emplace_back( "END_SCRIPT" );

  output_lines.emplace_back( "END_" + uniqueToken() );
}



Node::Node( std::vector< std::string > const & lines, int line_to_start_at ) {
  init();

  uint current_line = line_to_start_at;

  assert( lines[ current_line ] == "START_NODE" );

  while( lines[ ++current_line ] != "END_NODE" ){
    std::string const line = lines[ current_line ];

    if( line == "START_FLAGS" ) {
      std::stringstream ss;
      while( lines[ ++current_line ] != "END_FLAGS" ){
	//std::move this?
	ss << lines[ current_line ] << "\n";
      }
      user_rosetta_flags_ = ss.str();
      continue;
    }

    if( line == "START_NOTES" ) {
      while( lines[ ++current_line ] != "END_NOTES" ){
	//std::move this?
	notes_ += lines[ current_line ] + "\n";
      }
      continue;
    }

    if( line == "START_SCRIPT" ) {
      while( lines[ ++current_line ] != "END_SCRIPT" ){
	//std::move this?
	xml_script_ += lines[ current_line ] + "\n";
      }
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

    if( tokens[ 0 ] == "id" ) {
      id_ = std::stoi( tokens[ 1 ] );
      continue;
    }

    if( tokens[ 0 ] == "x" ) {
      x_ = std::stoi( tokens[ 1 ] );
      continue;
    }

    if( tokens[ 0 ] == "y" ) {
      y_ = std::stoi( tokens[ 1 ] );
      continue;
    }

    if( tokens[ 0 ] == "r" ) {
      red_ = std::stoi( tokens[ 1 ] );
      continue;
    }

    if( tokens[ 0 ] == "g" ) {
      green_ = std::stoi( tokens[ 1 ] );
      continue;
    }

    if( tokens[ 0 ] == "b" ) {
      blue_ = std::stoi( tokens[ 1 ] );
      continue;
    }

    if( tokens[ 0 ] == "command" ) {
      command_ = "";
      for( unsigned int i = 1; i < tokens.size(); ++i ) {
	command_ += tokens[ i ];
	if( i != tokens.size() - 1 ) {
	  command_ += " ";
	}
      }
      continue;
    }

    if( tokens[ 0 ] == "title" ) {
      title_ = tokens[ 1 ];
      for( unsigned int i = 2; i < tokens.size(); ++i ) {
	title_ += " " + tokens[ i ];
      }
      continue;
    }

    if( tokens[ 0 ] == "script" ) {
      xml_script_filename_ = ( tokens[ 1 ] == "1" );
      continue;
    }

    if( tokens[ 0 ] == "use_script_file" ) {
      use_script_file_ = ( tokens[ 1 ] == "1" );
      continue;
    }

    if( tokens[ 0 ] == "still_using_default_title" ) {
      still_using_default_title_ = ( tokens[ 1 ] == "1" );
      continue;
    }

    if( tokens[ 0 ] == "use_default_command" ) {
      continue;
    }
  } // for string line

}//load ctor

}//namespace graph
