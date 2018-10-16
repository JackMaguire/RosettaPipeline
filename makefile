# This is my first makefile ever so be nice!

#logic stolen from https://stackoverflow.com/questions/714100/os-detecting-makefile
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	include makefile_ubuntu
endif
ifeq ($(UNAME_S),Darwin)
	include makefile_mac
endif

#include makefile_mac

WT_FLAGS=-lwthttp -lwt -lboost_signals

########
# META #
########
all: graph save_and_load_graph.o bin

graph: Node.o Edge.o Graph.o

bin: save_and_load_graph

#########
# GRAPH #
#########

Node.o: src/graph/Node.cc src/graph/Edge.hh src/graph/Graph.fwd.hh
	${CXX} -c -o build/Node.o src/graph/Node.cc -O3 -Isrc -std=${std} ${WARN}

Edge.o: src/graph/Edge.cc src/graph/Node.hh src/graph/Graph.fwd.hh
	${CXX} -c -o build/Edge.o src/graph/Edge.cc -O3 -Isrc -std=${std} ${WARN}

Graph.o: src/graph/Graph.cc src/graph/Node.hh src/graph/Edge.hh
	${CXX} -c -o build/Graph.o src/graph/Graph.cc -O3 -Isrc -std=${std} ${WARN}

graph: build/Graph.o build/Edge.o build/Node.o
	ld -r build/Graph.o build/Edge.o build/Node.o -o build/graph.o

#########
# DEVEL #
#########

DummyPanel.o: src/devel/DummyPanel.hh
	${CXX} -c -o build/DummyPanel.o src/devel/DummyPanel.hh -O3 -Isrc -std=${std} ${WARN} ${WT_FLAGS}

########
# APPS #
########
save_and_load_graph.o: src/apps/proof_of_concept/save_and_load_graph.cc build/graph.o
	${CXX} -c -o build/save_and_load_graph.o src/apps/proof_of_concept/save_and_load_graph.cc -O3 -Isrc -std=${std} ${WARN}

testing_ground.o: build/graph.o
	${CXX} -c -o build/testing_ground.o src/apps/proof_of_concept/testing_ground.cc -O3 -Isrc -std=${std} ${WARN}

#######
# BIN #
#######

save_and_load_graph: build/graph.o build/save_and_load_graph.o
	${CXX} -o bin/save_and_load_graph build/save_and_load_graph.o build/graph.o -O3 -Isrc -std=${std} ${WARN}


testing_ground: build/graph.o build/testing_ground.o
	${CXX} -o bin/testing_ground build/testing_ground.o build/graph.o -O3 -Isrc -std=${std} ${WARN}
