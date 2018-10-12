
Node.o: src/graph/Node.cc
	clang++ -c -o build/Node.o src/graph/Node.cc -O3 -Isrc -std=c++17 -Wall

Edge.o: src/graph/Edge.cc
	clang++ -c -o build/Edge.o src/graph/Edge.cc -O3 -Isrc -std=c++17 -Wall

Graph.o: src/graph/Graph.cc
	clang++ -c -o build/Graph.o src/graph/Graph.cc -O3 -Isrc -std=c++17 -Wall
