# makefile for non-installed user
# author: xin cheng
# usage: change the following variable COLPACK_ROOT accordingly
#        delete OMP_FLAG=-fopenmp in MAC OS system
COLPACK_ROOT = ..
COLPACK_SRC = $(wildcard ${COLPACK_ROOT}/GraphColoring/*.cpp)
COLPACK_SRC+= $(wildcard ${COLPACK_ROOT}/Utilities/*.cpp)
COLPACK_SRC+= $(wildcard ${COLPACK_ROOT}/BipartiteGraphBicoloring/*.cpp)
COLPACK_SRC+= $(wildcard ${COLPACK_ROOT}/BipartiteGraphPartialColoring/*.cpp)

COLPACK_OBJ = $(COLPACK_SRC:%.cpp=%.o)
SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o) $(COLPACK_OBJ)
EXE = ColPack


# compiler
COMPILER = g++      # gnu
OMP_FLAG = -fopenmp 

#COMPILER = icc      # intel(R)
#OMP_FLAG = -openmp

# compile flags
CCFLAGS = -Wall -std=c++11 $(OMP_FLAG)  -Ofast #-O3 
# link flags
LDFLAGS = -Wall -std=c++11 $(OMP_FLAG)  -Ofast #-O3


INCLUDES = -I./
INCLUDES = -I${COLPACK_ROOT}/Main
INCLUDES+= -I${COLPACK_ROOT}/GraphColoring
INCLUDES+= -I${COLPACK_ROOT}/BipartiteGraphBicoloring
INCLUDES+= -I${COLPACK_ROOT}/BipartiteGraphPartialColoring
INCLUDES+= -I${COLPACK_ROOT}/Utilities
INCLUDES+= -I${COLPACK_ROOT}/Recovery


all: $(EXE)

%.o : %.cpp
	$(COMPILER) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(EXE): $(OBJ)
	$(COMPILER) $^ $(INCLUDES) $(LDFLAGS)  -o $@

clean:
	rm -f $(OBJ) $(EXE)

