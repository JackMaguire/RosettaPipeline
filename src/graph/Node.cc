#include <graph/Node.hh>
#include <graph/Edge.hh>

namespace graph {

void Node::init(){
  use_default_command_ = true;

  use_script_file_ = false;
  xml_script_filename_ = "script.xml";
  xml_script_ =
    "<ROSETTASCRIPTS>\n" +
    "    <SCOREFXNS>\n" +
    "    </SCOREFXNS>\n" +
    "    <RESIDUE_SELECTORS>\n" +
    "    </RESIDUE_SELECTORS>\n" +
    "    <TASKOPERATIONS>\n" +
    "    </TASKOPERATIONS>\n" +
    "    <FILTERS>\n" +
    "    </FILTERS>\n" +
    "    <MOVERS>\n" +
    "    </MOVERS>\n" +
    "    <PROTOCOLS>\n" +
    "    </PROTOCOLS>\n" +
    "    <OUTPUT />\n" +
    "</ROSETTASCRIPTS>\n";

  stage_ = 0;
  stage_is_valid_ = false;

  command_ = "TODO";
  notes_ = "";

  user_rosetta_flags_.emplace_back( "# Keep in mind that all commands will be run one directory deeper." );
  user_rosetta_flags_.emplace_back(
    "# You would need to pass '-s ../pose.pdb' instead of '-s pose.pdb'" );
  user_rosetta_flags_.emplace_back( "" );
  user_rosetta_flags_.emplace_back( "# -nstruct 1" );

  //TODO
  //command_ = Options.getDefaultRunCommand();
}

}
