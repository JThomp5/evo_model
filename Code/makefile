model: *.cc *.h
	${GXX} main.cc VertexGenerator.cc Vertex.cc -o model -L../../Libraries/Files -lfiles -L../../Libraries/Random -lRandom -L../../Libraries/Params -lParams -g -std=c++11

vgtest: vgtest.cc VertexGenerator.h
	${GXX} vgtest.cc -o vgtest -g -std=c++11
