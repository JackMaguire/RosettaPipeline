#include <Wt/WApplication>

#include <graph/Graph.hh>
#include <view/graph_view.hh>
#include <view/top_level_view.hh>

#include <memory>

using namespace Wt;

class GraphApplication: public WApplication
{
public:
  GraphApplication(const WEnvironment &env): WApplication(env) {
    setTitle("Graph example");

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

    //view::GraphWidget * widget = new view::GraphWidget( root(), graph );
    view::TopWidget * widget = new view::TopWidget( root(), graph );
  }

//private:
  //std::unique_ptr< PaintExample >;
};

WApplication * createApplication( const WEnvironment&  env )
{
  return new GraphApplication(env);
}

int main(int argc, char **argv)
{
  return WRun(argc, argv, &createApplication);
}
