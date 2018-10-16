# This is my first makefile ever so be nice!

include makefile_ubuntu
#include makefile_mac

########
# META #
########
all: Node.o Edge.o Graph.o save_and_load_graph.o bin

bin: save_and_load_graph

###########
# OBJECTS #
###########

Node.o: src/graph/Node.cc src/graph/Edge.hh src/graph/Graph.fwd.hh
	${CXX} -c -o build/Node.o src/graph/Node.cc -O3 -Isrc -std=${std} ${WARN}

Edge.o: src/graph/Edge.cc src/graph/Node.hh src/graph/Graph.fwd.hh
	${CXX} -c -o build/Edge.o src/graph/Edge.cc -O3 -Isrc -std=${std} ${WARN}

Graph.o: src/graph/Graph.cc src/graph/Node.hh src/graph/Edge.hh
	${CXX} -c -o build/Graph.o src/graph/Graph.cc -O3 -Isrc -std=${std} ${WARN}

save_and_load_graph.o: src/apps/proof_of_concept/save_and_load_graph.cc src/graph/Graph.hh
	${CXX} -c -o build/save_and_load_graph.o src/apps/proof_of_concept/save_and_load_graph.cc -O3 -Isrc -std=${std} ${WARN}

#######
# BIN #
#######

save_and_load_graph: build/Node.o build/Edge.o build/Graph.o build/save_and_load_graph.o
	${CXX} -o bin/save_and_load_graph build/Node.o build/Edge.o build/Graph.o build/save_and_load_graph.o -O3 -Isrc -std=${std} ${WARN}
