#include <serialization.hh>

#include <graph/Graph.hh>
#include <Options.hh>

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <memory>
#include <cassert>

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
  ArchiverImpl archiver;
  archiver.add_element( "START", "SERIALIZATION" );
  options.save( archiver );
  graph.saveSelfNodesAndEdges( archiver );
  archiver.add_element( "END", "SERIALIZATION" );
  return archiver.get_final_string();
}




////////
//LOAD//
////////

namespace {

class UnarchiverImpl : public Unarchiver {
public:
  UnarchiverImpl( std::string const & filename ) :
    in_stream_( filename )
  {
    archive_ = std::make_unique< cereal::BinaryOutputArchive >( in_stream_ );
  }

  ~UnarchiverImpl(){
    in_stream_.close();
  }

  ArchiveElement get_next_element() override {
    ArchiveElementImpl element( "", "" );
    (*archive_)( element );

    assert( element.token != "END" || element.value != "SERIALIZATION" );

    return element;
  }

private:
  std::ifstream in_stream_;
  BinaryOutputArchiveUP archive_;
};

}//anonymous namespace

std::string load_file(
  std::string const & filename,
  graph::Graph & graph,
  Options & options
){
  try {
    UnarchiverImpl unarchiver( filename );
    ArchiveElement first_element = unarchiver.get_next_element();
    if( first_element.token != "START" || first_element.value != "SERIALIZATION" ){
      return filename + " is either corrupted or out of date.";
    }

    options.load( unarchiver );
    //graph.loadSelfNodesAndEdges( unarchiver, options );

    return "load successful";
  } catch (...) {
    return "Unable to load from " + filename;
  }
}


}//namespace
