all: Node.o Edge.o Graph.o save_and_load_graph.o save_and_load_graph

Node.o: src/graph/Node.cc
	clang++ -c -o build/Node.o src/graph/Node.cc -O3 -Isrc -std=c++17 -Wall

Edge.o: src/graph/Edge.cc
	clang++ -c -o build/Edge.o src/graph/Edge.cc -O3 -Isrc -std=c++17 -Wall

Graph.o: src/graph/Graph.cc
	clang++ -c -o build/Graph.o src/graph/Graph.cc -O3 -Isrc -std=c++17 -Wall

save_and_load_graph.o: src/apps/proof_of_concept/save_and_load_graph.cc
	clang++ -c -o build/save_and_load_graph.o src/apps/proof_of_concept/save_and_load_graph.cc -O3 -Isrc -std=c++17 -Wall

#######
# BIN #
#######

save_and_load_graph: build/Node.o build/Edge.o build/Graph.o build/save_and_load_graph.o
	clang++ -o bin/save_and_load_graph build/Node.o build/Edge.o build/Graph.o build/save_and_load_graph.o -O3 -Isrc -std=c++17 -Wall
