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

GEN=-flto -O3 -Isrc -std=${std} ${WARN} ${EXTRA} -lstdc++fs -DROSETTA_PIPELINE_PWD=$(shell pwd)
#GEN=-g -Isrc -std=${std} ${WARN} ${EXTRA} -lstdc++fs -DROSETTA_PIPELINE_PWD=$(shell pwd)

WT_FLAGS=-lwthttp2 -lwt2 -lboost_signals -DBOOST_SIGNALS_NO_DEPRECATION_WARNING -Iwt_src -L. -Wl,-rpath,.

########
# META #
########
all: Options.o graph compile view apps bin

all_no_wt: Options.o graph compile

graph: Node.o Edge.o Graph.o

bin: graph_view_app

###########
# OPTIONS #
###########

Options.o: src/Options.hh
	${CXX} -c -o build/Options.o src/Options.cc ${GEN} 

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

GraphToolbarWidget.o: src/view/GraphToolbarWidget.hh graph
	${CXX} -c -o build/GraphToolbarWidget.o src/view/GraphToolbarWidget.cc ${GEN} ${WT_FLAGS}

NodeWidget.o: src/view/NodeWidget.hh graph
	${CXX} -c -o build/NodeWidget.o src/view/NodeWidget.cc ${GEN} ${WT_FLAGS}

EdgeWidget.o: src/view/EdgeWidget.hh graph
	${CXX} -c -o build/EdgeWidget.o src/view/EdgeWidget.cc ${GEN} ${WT_FLAGS}

OptionsWidget.o: src/view/OptionsWidget.cc
	${CXX} -c -o build/OptionsWidget.o src/view/OptionsWidget.cc ${GEN} ${WT_FLAGS}

SaveWidget.o: src/view/SaveWidget.cc
	${CXX} -c -o build/SaveWidget.o src/view/SaveWidget.cc ${GEN} ${WT_FLAGS}

LoadWidget.o: src/view/LoadWidget.cc
	${CXX} -c -o build/LoadWidget.o src/view/LoadWidget.cc ${GEN} ${WT_FLAGS}

PublishWidget.o: src/view/PublishWidget.cc
	${CXX} -c -o build/PublishWidget.o src/view/PublishWidget.cc ${GEN} ${WT_FLAGS}

CompileWidget.o: src/view/CompileWidget.cc compile
	${CXX} -c -o build/CompileWidget.o src/view/CompileWidget.cc ${GEN} ${WT_FLAGS}

WelcomeWidget.o: src/view/WelcomeWidget.cc
	${CXX} -c -o build/WelcomeWidget.o src/view/WelcomeWidget.cc ${GEN} ${WT_FLAGS}

ExamplesWidget.o: src/view/ExamplesWidget.cc
	${CXX} -c -o build/ExamplesWidget.o src/view/ExamplesWidget.cc ${GEN} ${WT_FLAGS}

ExtraFileWidget.o: src/view/ExtraFileWidget.cc
	${CXX} -c -o build/ExtraFileWidget.o src/view/ExtraFileWidget.cc ${GEN} ${WT_FLAGS}

RightViewWidget.o: src/view/RightViewWidget.cc
	${CXX} -c -o build/RightViewWidget.o src/view/RightViewWidget.cc ${GEN} ${WT_FLAGS}

LeftViewWidget.o: src/view/LeftViewWidget.cc
	${CXX} -c -o build/LeftViewWidget.o src/view/LeftViewWidget.cc ${GEN} ${WT_FLAGS}

TopWidget.o: src/view/TopWidget.cc
	${CXX} -c -o build/TopWidget.o src/view/TopWidget.cc ${GEN} ${WT_FLAGS}

view: GraphWidget.o GraphToolbarWidget.o NodeWidget.o EdgeWidget.o TopWidget.o LeftViewWidget.o RightViewWidget.o CompileWidget.o \
	 LoadWidget.o SaveWidget.o OptionsWidget.o WelcomeWidget.o ExamplesWidget.o ExtraFileWidget.o PublishWidget.o
	ld -r build/GraphWidget.o build/GraphToolbarWidget.o build/TopWidget.o build/RightViewWidget.o build/LeftViewWidget.o \
		build/NodeWidget.o build/EdgeWidget.o build/SaveWidget.o build/OptionsWidget.o build/WelcomeWidget.o build/PublishWidget.o\
		build/LoadWidget.o build/CompileWidget.o build/ExamplesWidget.o build/ExtraFileWidget.o -o build/view.o -arch ${ARCH}


########
# APPS #
########
GraphApplication: src/apps/proof_of_concept/graph_view_app.cc
	${CXX} -c -o build/graph_view_app.o src/apps/proof_of_concept/graph_view_app.cc ${GEN} ${WT_FLAGS}

apps: GraphApplication

#######
# BIN #
#######

graph_view_app: GraphApplication view Options.o
	${CXX} -o bin/graph_view_app build/graph_view_app.o build/graph.o build/view.o build/Options.o build/compile.o ${GEN} ${WT_FLAGS}
