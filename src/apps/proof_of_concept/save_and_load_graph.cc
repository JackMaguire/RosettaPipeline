#include <graph/Graph.hh>
#include <graph/Node.hh>
#include <graph/Edge.hh>

#include <iostream>

int main(){
  graph::Graph graph;
  std::cout << "created graph with " << graph.nodes().size() << " nodes" << std::endl;
}
