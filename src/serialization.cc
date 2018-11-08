#include <serialization.hh>

#include <graph/Graph.hh>
#include <Options.hh>

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <memory>

//CEREAL
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>

using BinaryOutputArchiveUP = std::unique_ptr< cereal::BinaryOutputArchive >;

namespace serialization {

struct ArchiveElementImpl : public ArchiveElement {
  ArchiveElementImpl(
    std::string const & t,
    std::string const & v
  ):
    ArchiveElement( t, v )
  {}

  template < class Archive >
  void save( Archive & ar ) const {
    ar( token );
    ar( value );
  }
      
  template < class Archive >
  void load( Archive & ar ) {
    ar( token );
    ar( value );
  }
};


////////
//SAVE//
////////

namespace {

class ArchiverImpl : public Archiver {
public:
  ArchiverImpl(){
    archive_ = std::make_unique< cereal::BinaryOutputArchive >( ss_ );
  }

  ~ArchiverImpl() = default;

  void add_element( std::string const & token, std::string const & value ) override {
    ArchiveElementImpl element( token, value );
    ( *archive_ )( element );
  }

  std::string
  get_final_string(){
    return ss_.str();
  }

private:
  std::stringstream ss_;
  BinaryOutputArchiveUP archive_;
};

}

std::string save(
  Options const & options,
  graph::Graph const & graph
) {
  /*
    OLD METHOD:
    std::vector< std::string > save_lines;
    options.save( save_lines );
    graph.saveSelfNodesAndEdges( save_lines );
    
    std::stringstream ss;
    for( std::string const & line : save_lines ){
    ss << line << "\n";
    }
    return ss.str();
  */
  //std::ofstream os("out.cereal", std::ios::binary);
  ArchiverImpl archiver;
  options.save( archiver );
  //graph.save( archiver );
  return archiver.get_final_string();
}




////////
//LOAD//
////////

namespace {

/*class UnarchiverImpl : public Unarchiver {
public:
  ArchiveElement const & get_next_element() override {

  }
};*/

std::vector< std::string > get_file_lines( std::string const & filename ){
  std::vector< std::string > myLines;
  std::ifstream myFile( filename );
  if( myFile.good() ){
    for( std::string line; std::getline( myFile, line ); ) {
      myLines.push_back(line);
    }
  }
  return myLines;
}

std::string load_file(
  std::vector< std::string > const & file_lines,
  graph::Graph & graph,
  Options & options
){
  //First, options
  int current_line = 0;
  if( file_lines[ current_line ] != "START_OPTIONS" ) {
    return "Error, expected first line to say \"START_OPTIONS\"";
  }
  current_line = options.load( file_lines, current_line ) + 1;

  //Graph
  if( file_lines[ current_line ] != "START_GRAPH" ) {
    return "Error, expected first line to say \"START_OPTIONS\"";
  }
  current_line = graph.loadSelfNodesAndEdges( file_lines, current_line, options ) + 1;

  return "load successful";
}

}

std::string load_file(
  std::string const & filename,
  graph::Graph & graph,
  Options & options
){
  return load_file( get_file_lines( filename ), graph, options );
}


}//namespace
