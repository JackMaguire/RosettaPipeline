#include <Wt/WApplication.h>
#include <Wt/WBootstrapTheme.h>

#include <graph/Graph.hh>
//#include <widgets/GraphWidget.hh>
#include <widgets/TopWidget.hh>

#include <memory>
#include <iostream>
#include <string>
#include <vector>

#include <Options.hh>

using namespace Wt;

std::unique_ptr< Wt::WApplication > createApplication( Wt::WEnvironment const & env ) {
  OptionsSP options = std::make_shared< Options >();

  graph::GraphSP graph = std::make_shared< graph::Graph >();
  graph::NodeSP previous = 0;
  for( int i=0; i < 3; ++i ){
    graph::NodeSP node = std::make_shared< graph::Node >( * options, 20, i*15, graph.get() );
    graph->addNode( node );
    if( i > 0 ){
      graph->addEdge( previous, node );
    }
    previous = node;
  }

  std::unique_ptr< Wt::WApplication > app = Wt::cpp14::make_unique< Wt::WApplication >( env );
  app->setTitle( "Rosetta Pipeline" );
  app->root()->setStyleClass( "root" );
  app->root()->addWidget( Wt::cpp14::make_unique< widgets::TopWidget >( std::move( graph ), std::move( options ) ) );
  app->setCssTheme( "polished" );
  app->setTheme( std::make_shared< Wt::WBootstrapTheme >() ) ;

  //http://0.0.0.0:8080/?_=key/12345/ gives:
  std::cout << app->url() << std::endl; // /?wtd=wwjZ9KkfGWFwg6T8
  std::cout << app->internalSubPath( "" ) << std::endl; // /key/12345/

  std::vector< std::string > tokens;
  {//https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    std::string const internal_path = app->internalSubPath( "" );
    std::size_t prev = 0, pos = 0;
    do {
      pos = internal_path.find( "/", prev );
      if ( pos == string::npos ) pos = internal_path.length();
      std::string token = internal_path.substr(prev, pos-prev);
      if( !token.empty() ) tokens.push_back( std::move( token ) );
      prev = pos + 1;
    }
    while( prev < internal_path.length() );
  }

  return app;
}

int main( int argc, char **argv )
{
  return WRun( argc, argv, &createApplication );
}
