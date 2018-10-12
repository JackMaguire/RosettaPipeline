#include <graph/Graph.hh>
#include <graph/Node.hh>
#include <graph/Edge.hh>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(){
  graph::Graph graph;
  std::cout << "created graph with " << graph.nodes().size() << " nodes" << std::endl;

  auto const node1 = std::make_shared< graph::Node >( "Test1", 0, 0 );
  auto const node2 = std::make_shared< graph::Node >( "Test2", 0, 0 );
  graph.addNode( node1 );
  graph.addNode( node2 );
  auto const edge12 = graph.addEdge( node1, node2 );

  std::vector< std::string > out_lines;
  graph.saveSelfNodesAndEdges( out_lines );

  std::ofstream out;
  out.open( "test.pipeline", std::ofstream::out );
  for( std::string const & s : out_lines ){
    out << s << std::endl;
  }
  out.close();


  graph::Graph graph2;
  graph2.loadSelfNodesAndEdges( out_lines, 0 );

  std::cout << "trait\tbefore\tafter\n" <<
    "num_nodes\t" << graph.nodes().size() << "\t" << graph2.nodes().size() <<
    std::endl;
}
