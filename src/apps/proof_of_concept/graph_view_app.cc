#include <Wt/WApplication.h>
#include <Wt/WBootstrapTheme.h>

#include <graph/Graph.hh>
//#include <view/GraphWidget.hh>
#include <view/TopWidget.hh>

#include <memory>

using namespace Wt;

std::unique_ptr< Wt::WApplication > createApplication( Wt::WEnvironment const & env ) {
  graph::GraphSP graph = std::make_shared< graph::Graph >();
  graph::NodeSP previous = 0;
  for( int i=0; i < 3; ++i ){
    graph::NodeSP node = std::make_shared< graph::Node >( 20, i*15);
    graph->addNode( node );
    if( i > 0 ){
      graph->addEdge( previous, node );
    }
    previous = node;
  }

  std::unique_ptr< Wt::WApplication > app = Wt::cpp14::make_unique< Wt::WApplication >( env );
  app->setTitle( "Rosetta Pipeline" );
  app->root()->setStyleClass( "root" );
  app->root()->addWidget( Wt::cpp14::make_unique< view::TopWidget >( graph ) );
  //app->setCssTheme("polished");
  app->setTheme( new Wt::WBootstrapTheme() ) ;
  //app->useStyleSheet("dragdrop.css");
  return app;
}

int main( int argc, char **argv )
{
  return WRun( argc, argv, &createApplication );
}
