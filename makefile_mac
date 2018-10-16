# This is my first makefile ever so be nice!

########
# META #
########
all: Node.o Edge.o Graph.o save_and_load_graph.o bin

bin: save_and_load_graph

###########
# OBJECTS #
###########

Node.o: src/graph/Node.cc src/graph/Edge.hh src/graph/Graph.fwd.hh
	clang++ -c -o build/Node.o src/graph/Node.cc -O3 -Isrc -std=c++1z -Wall

Edge.o: src/graph/Edge.cc src/graph/Node.hh src/graph/Graph.fwd.hh
	clang++ -c -o build/Edge.o src/graph/Edge.cc -O3 -Isrc -std=c++1z -Wall

Graph.o: src/graph/Graph.cc src/graph/Node.hh src/graph/Edge.hh
	clang++ -c -o build/Graph.o src/graph/Graph.cc -O3 -Isrc -std=c++1z -Wall

save_and_load_graph.o: src/apps/proof_of_concept/save_and_load_graph.cc src/graph/Graph.hh
	clang++ -c -o build/save_and_load_graph.o src/apps/proof_of_concept/save_and_load_graph.cc -O3 -Isrc -std=c++1z -Wall

#######
# BIN #
#######

save_and_load_graph: build/Node.o build/Edge.o build/Graph.o build/save_and_load_graph.o
	clang++ -o bin/save_and_load_graph build/Node.o build/Edge.o build/Graph.o build/save_and_load_graph.o -O3 -Isrc -std=c++1z -Wall
