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
#WT_FLAGS=-lwthttp -lwt -lboost_signals -DBOOST_SIGNALS_NO_DEPRECATION_WARNING
WT_FLAGS=-lwthttp2 -lwt2 -lboost_signals -DBOOST_SIGNALS_NO_DEPRECATION_WARNING -Iwt_src -L. -Wl,-rpath,.

########
# META #
########
all: options graph compile view apps bin

all_no_wt: options graph compile

graph: Node.o Edge.o Graph.o

bin: save_and_load_graph graph_view_app tab_testing_app

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

GraphWidget.o: src/view/GraphWidget.hh graph
	${CXX} -c -o build/GraphWidget.o src/view/GraphWidget.cc ${GEN} ${WT_FLAGS}

NodeWidget.o: src/view/NodeWidget.hh graph
	${CXX} -c -o build/NodeWidget.o src/view/NodeWidget.cc ${GEN} ${WT_FLAGS}

EdgeWidget.o: src/view/EdgeWidget.hh graph
	${CXX} -c -o build/EdgeWidget.o src/view/EdgeWidget.cc ${GEN} ${WT_FLAGS}

EditWidget.o: src/view/EditWidget.cc
	${CXX} -c -o build/EditWidget.o src/view/EditWidget.cc ${GEN} ${WT_FLAGS}

SaveWidget.o: src/view/SaveWidget.cc
	${CXX} -c -o build/SaveWidget.o src/view/SaveWidget.cc ${GEN} ${WT_FLAGS}

LoadWidget.o: src/view/LoadWidget.cc
	${CXX} -c -o build/LoadWidget.o src/view/LoadWidget.cc ${GEN} ${WT_FLAGS}

CompileWidget.o: src/view/CompileWidget.cc compile
	${CXX} -c -o build/CompileWidget.o src/view/CompileWidget.cc ${GEN} ${WT_FLAGS}

RightViewWidget.o: src/view/RightViewWidget.cc
	${CXX} -c -o build/RightViewWidget.o src/view/RightViewWidget.cc ${GEN} ${WT_FLAGS}

TopWidget.o: src/view/TopWidget.cc
	${CXX} -c -o build/TopWidget.o src/view/TopWidget.cc ${GEN} ${WT_FLAGS}

view: GraphWidget.o NodeWidget.o EdgeWidget.o TopWidget.o RightViewWidget.o EditWidget.o CompileWidget.o LoadWidget.o SaveWidget.o
	ld -r build/GraphWidget.o build/TopWidget.o build/RightViewWidget.o build/EditWidget.o \
		build/NodeWidget.o build/EdgeWidget.o build/SaveWidget.o \
		build/LoadWidget.o build/CompileWidget.o -o build/view.o -arch ${ARCH}


########
# APPS #
########
save_and_load_graph.o: src/apps/proof_of_concept/save_and_load_graph.cc graph global_data
	${CXX} -c -o build/save_and_load_graph.o src/apps/proof_of_concept/save_and_load_graph.cc ${GEN}

GraphApplication: src/apps/proof_of_concept/graph_view_app.cc
	${CXX} -c -o build/graph_view_app.o src/apps/proof_of_concept/graph_view_app.cc ${GEN} ${WT_FLAGS}

tab_testing: src/apps/proof_of_concept/tab_testing.cc
	${CXX} -c -o build/tab_testing.o src/apps/proof_of_concept/tab_testing.cc ${GEN} ${WT_FLAGS}

apps: save_and_load_graph.o GraphApplication tab_testing

#######
# BIN #
#######

save_and_load_graph: save_and_load_graph.o graph global_data
	${CXX} -o bin/save_and_load_graph build/save_and_load_graph.o build/global_data.o build/graph.o ${GEN}

graph_view_app: GraphApplication view 
	${CXX} -o bin/graph_view_app build/graph_view_app.o build/graph.o build/view.o build/options.o build/compile.o ${GEN} ${WT_FLAGS}

tab_testing_app: tab_testing
	${CXX} -o bin/tab_testing_app build/tab_testing.o ${GEN} ${WT_FLAGS}
