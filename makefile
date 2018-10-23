# This is my first makefile ever so be nice!

#logic stolen from https://stackoverflow.com/questions/714100/os-detecting-makefile
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	include makefile_ubuntu
	EXTRA=
endif
ifeq ($(UNAME_S),Darwin)
	include makefile_mac
	EXTRA=-arch ${ARCH}
endif

#include makefile_mac

GEN=-flto -O3 -Isrc -std=${std} ${WARN} ${EXTRA}
WT_FLAGS=-lwthttp -lwt -lboost_signals -DBOOST_SIGNALS_NO_DEPRECATION_WARNING

########
# META #
########
all: options graph compile view apps bin

all_no_wt: options graph compile

graph: Node.o Edge.o Graph.o

bin: save_and_load_graph graph_view_app

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

########
# VIEW #
########

GraphWidget.o: src/view/graph_view.hh graph
	${CXX} -c -o build/GraphWidget.o src/view/graph_view.cc ${GEN} ${WT_FLAGS}

RightViewWidget.o: src/view/RightViewWidget.cc
	${CXX} -c -o build/RightViewWidget.o src/view/RightViewWidget.cc ${GEN} ${WT_FLAGS}

TopWidget.o: src/view/top_level_view.cc
	${CXX} -c -o build/TopWidget.o src/view/top_level_view.cc ${GEN} ${WT_FLAGS}

view: GraphWidget.o TopWidget.o RightViewWidget.o
	ld -r build/GraphWidget.o build/TopWidget.o build/RightViewWidget.o -o build/view.o -arch ${ARCH}


########
# APPS #
########
save_and_load_graph.o: src/apps/proof_of_concept/save_and_load_graph.cc graph global_data
	${CXX} -c -o build/save_and_load_graph.o src/apps/proof_of_concept/save_and_load_graph.cc ${GEN}

GraphApplication: src/apps/proof_of_concept/graph_view_app.cc
	${CXX} -c -o build/graph_view_app.o src/apps/proof_of_concept/graph_view_app.cc ${GEN} ${WT_FLAGS}

apps: save_and_load_graph.o GraphApplication

#######
# BIN #
#######

save_and_load_graph: save_and_load_graph.o graph global_data
	${CXX} -o bin/save_and_load_graph build/save_and_load_graph.o build/global_data.o build/graph.o ${GEN}

graph_view_app: GraphApplication view 
	${CXX} -o bin/graph_view_app build/graph_view_app.o build/graph.o build/view.o build/options.o ${GEN} ${WT_FLAGS}
