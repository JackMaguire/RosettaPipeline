#include <Wt/WApplication>

#include <view/graph_view.hh>

using namespace Wt;

class GraphApplication: public WApplication
{
public:
  GraphApplication(const WEnvironment &env): WApplication(env) {
    setTitle("Graph example");
    
    //useStyleSheet("painting.css");
    
    new view::GraphWidget( root() );
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
