#include <Wt2/WApplication.h>

#include <graph/Graph.hh>
#include <view/graph_view.hh>
#include <view/top_level_view.hh>

#include <memory>

using namespace Wt;

std::unique_ptr<WApplication> createApplication(const WEnvironment& env)
{
  graph::GraphSP graph = std::make_shared< graph::Graph >();
  graph::NodeSP previous = 0;
  for( int i=0; i < 3; ++i ){
    graph::NodeSP node = std::make_shared< graph::Node >( i*10, i*5);
    graph->addNode( node );
    if( i > 0 ){
      graph->addEdge( previous, node );
    }
    previous = node;
  }

  std::unique_ptr<WApplication> app = Wt::cpp14::make_unique<WApplication>(env);
  app->setTitle("Rosetta Pipeline");
  app->root()->setStyleClass("root");
  app->root()->addWidget( cpp14::make_unique< view::TopWidget >( graph ) );
  //app->useStyleSheet("dragdrop.css");
  return app;
}

int main(int argc, char **argv)
{
  return WRun(argc, argv, &createApplication);
}
