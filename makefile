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
all: misc graph widgets apps bin

graph: Node.o Edge.o Graph.o

bin: graph_view_app

########
# MISC #
########

Options.o: src/Options.hh
	${CXX} -c -o build/Options.o src/Options.cc ${GEN} 

compile.o: graph
	${CXX} -c -o build/compile.o src/compile/compile.cc ${GEN} 

publishing.o: src/publishing.cc
	${CXX} -c -o build/publishing.o src/publishing.cc ${GEN}

serialization.o: src/serialization.cc
	${CXX} -c -o build/serialization.o src/serialization.cc ${GEN}

misc: Options.o compile.o publishing.o serialization.o
	ld -r build/Options.o build/compile.o build/publishing.o build/serialization.o -o build/misc.o -arch ${ARCH}

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
# WIDGETS #
###########

GraphWidget.o: src/widgets/GraphWidget.hh graph
	${CXX} -c -o build/GraphWidget.o src/widgets/GraphWidget.cc ${GEN} ${WT_FLAGS}

GraphToolbarWidget.o: src/widgets/GraphToolbarWidget.hh graph
	${CXX} -c -o build/GraphToolbarWidget.o src/widgets/GraphToolbarWidget.cc ${GEN} ${WT_FLAGS}

PublicationBrowserDialog.o: src/widgets/PublicationBrowserDialog.hh graph
	${CXX} -c -o build/PublicationBrowserDialog.o src/widgets/PublicationBrowserDialog.cc ${GEN} ${WT_FLAGS}

NodeWidget.o: src/widgets/NodeWidget.hh graph
	${CXX} -c -o build/NodeWidget.o src/widgets/NodeWidget.cc ${GEN} ${WT_FLAGS}

EdgeWidget.o: src/widgets/EdgeWidget.hh graph
	${CXX} -c -o build/EdgeWidget.o src/widgets/EdgeWidget.cc ${GEN} ${WT_FLAGS}

OptionsWidget.o: src/widgets/OptionsWidget.cc
	${CXX} -c -o build/OptionsWidget.o src/widgets/OptionsWidget.cc ${GEN} ${WT_FLAGS}

SaveWidget.o: src/widgets/SaveWidget.cc
	${CXX} -c -o build/SaveWidget.o src/widgets/SaveWidget.cc ${GEN} ${WT_FLAGS}

LoadWidget.o: src/widgets/LoadWidget.cc
	${CXX} -c -o build/LoadWidget.o src/widgets/LoadWidget.cc ${GEN} ${WT_FLAGS}

PublishWidget.o: src/widgets/PublishWidget.cc
	${CXX} -c -o build/PublishWidget.o src/widgets/PublishWidget.cc ${GEN} ${WT_FLAGS}

CompileWidget.o: src/widgets/CompileWidget.cc compile
	${CXX} -c -o build/CompileWidget.o src/widgets/CompileWidget.cc ${GEN} ${WT_FLAGS}

WelcomeWidget.o: src/widgets/WelcomeWidget.cc
	${CXX} -c -o build/WelcomeWidget.o src/widgets/WelcomeWidget.cc ${GEN} ${WT_FLAGS}

ExamplesWidget.o: src/widgets/ExamplesWidget.cc
	${CXX} -c -o build/ExamplesWidget.o src/widgets/ExamplesWidget.cc ${GEN} ${WT_FLAGS}

ExtraFileWidget.o: src/widgets/ExtraFileWidget.cc
	${CXX} -c -o build/ExtraFileWidget.o src/widgets/ExtraFileWidget.cc ${GEN} ${WT_FLAGS}

RightViewWidget.o: src/widgets/RightViewWidget.cc
	${CXX} -c -o build/RightViewWidget.o src/widgets/RightViewWidget.cc ${GEN} ${WT_FLAGS}

LeftViewWidget.o: src/widgets/LeftViewWidget.cc
	${CXX} -c -o build/LeftViewWidget.o src/widgets/LeftViewWidget.cc ${GEN} ${WT_FLAGS}

TopWidget.o: src/widgets/TopWidget.cc
	${CXX} -c -o build/TopWidget.o src/widgets/TopWidget.cc ${GEN} ${WT_FLAGS}

widgets: GraphWidget.o GraphToolbarWidget.o NodeWidget.o EdgeWidget.o TopWidget.o LeftViewWidget.o RightViewWidget.o CompileWidget.o \
	 LoadWidget.o SaveWidget.o OptionsWidget.o WelcomeWidget.o ExamplesWidget.o ExtraFileWidget.o PublishWidget.o PublicationBrowserDialog.o
	ld -r build/GraphWidget.o build/GraphToolbarWidget.o build/TopWidget.o build/RightViewWidget.o build/LeftViewWidget.o build/PublicationBrowserDialog.o\
		build/NodeWidget.o build/EdgeWidget.o build/SaveWidget.o build/OptionsWidget.o build/WelcomeWidget.o build/PublishWidget.o\
		build/LoadWidget.o build/CompileWidget.o build/ExamplesWidget.o build/ExtraFileWidget.o -o build/widgets.o -arch ${ARCH}


########
# APPS #
########
GraphApplication: src/apps/proof_of_concept/graph_view_app.cc
	${CXX} -c -o build/graph_view_app.o src/apps/proof_of_concept/graph_view_app.cc ${GEN} ${WT_FLAGS}

apps: GraphApplication

#######
# BIN #
#######

graph_view_app: GraphApplication widgets misc
	${CXX} -o bin/graph_view_app build/graph_view_app.o build/graph.o build/widgets.o build/misc.o ${GEN} ${WT_FLAGS}
