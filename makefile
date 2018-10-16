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

GEN=-flto -O3 -Isrc -std=${std} ${WARN} -arch ${ARCH}
WT_FLAGS=-lwthttp -lwt -lboost_signals

########
# META #
########
all: options graph compile devel apps bin

all_no_wt: options graph compile

graph: Node.o Edge.o Graph.o

bin: save_and_load_graph

###########
# OPTIONS #
###########

options: src/global_data/options.hh
	${CXX} -c -o build/options.o src/global_data/options.cc ${GEN} 

global_data: options
	cp build/options.o build/global_data.o

#########
# GRAPH #
#########

Node.o: src/graph/Node.cc src/graph/Edge.hh src/graph/Graph.fwd.hh
	${CXX} -c -o build/Node.o src/graph/Node.cc ${GEN}

Edge.o: src/graph/Edge.cc src/graph/Node.hh src/graph/Graph.fwd.hh
	${CXX} -c -o build/Edge.o src/graph/Edge.cc ${GEN}

Graph.o: src/graph/Graph.cc src/graph/Node.hh src/graph/Edge.hh
	${CXX} -c -o build/Graph.o src/graph/Graph.cc ${GEN}

graph: Graph.o Edge.o Node.o
	ld -r build/Graph.o build/Edge.o build/Node.o -o build/graph.o -arch ${ARCH}

###########
# COMPILE #
###########

compile: graph
	${CXX} -c -o build/compile.o src/compile/compile.cc ${GEN} 

#########
# DEVEL #
#########

DummyPanel.o: src/devel/DummyPanel.hh
	${CXX} -c -o build/DummyPanel.o src/devel/DummyPanel.hh ${GEN} ${WT_FLAGS}

devel: DummyPanel.o
	cp build/DummyPanel.o build/devel.o

#devel: build/DummyPanel.o
#	ld -r build/DummyPanel.o -o build/devel.o

########
# APPS #
########
save_and_load_graph.o: src/apps/proof_of_concept/save_and_load_graph.cc graph global_data
	${CXX} -c -o build/save_and_load_graph.o src/apps/proof_of_concept/save_and_load_graph.cc ${GEN}

testing_ground.o: graph
	${CXX} -c -o build/testing_ground.o src/apps/proof_of_concept/testing_ground.cc ${GEN}

apps: save_and_load_graph.o testing_ground.o

#######
# BIN #
#######

save_and_load_graph: build/save_and_load_graph.o graph global_data
	${CXX} -o bin/save_and_load_graph build/save_and_load_graph.o build/global_data.o build/graph.o ${GEN}


testing_ground: build/graph.o build/testing_ground.o build/devel.o
	${CXX} -o bin/testing_ground build/testing_ground.o build/graph.o build/devel.o ${GEN}
