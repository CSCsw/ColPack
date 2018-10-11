# makefile for non-installed user
# author: xin cheng
# usage: change the following variable COLPACK_ROOT accordingly
#        delete OMP_FLAG=-fopenmp in MAC OS system
COLPACK_ROOT = ../..
COLPACK_SRC = $(wildcard ${COLPACK_ROOT}/SRC/GeneralGraphColoring/*.cpp)
COLPACK_SRC+= $(wildcard ${COLPACK_ROOT}/SRC/Utilities/*.cpp)
COLPACK_SRC+= $(wildcard ${COLPACK_ROOT}/SRC/BipartiteGraphBicoloring/*.cpp)
COLPACK_SRC+= $(wildcard ${COLPACK_ROOT}/SRC/BipartiteGraphPartialColoring/*.cpp)
COLPACK_SRC+= $(wildcard ${COLPACK_ROOT}/SRC/SMPGC/*.cpp)
COLPACK_SRC+= $(wildcard ${COLPACK_ROOT}/SRC/PartialD2SMPGC/*.cpp)
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
CCFLAGS = -Wall -std=c++11 $(OMP_FLAG)  -Ofast -m64 #-D PD2_VARIFY -D PD2_MASKWIDE_64 -D DEBUG_JP_PROFILE #-D PRINT_DETAILED_STATS_#-O3 
# link flags
LDFLAGS = -Wall -std=c++11 $(OMP_FLAG)  -Ofast -m64 #-D PD2_VARIFY -D PD2_MASKWIDE_64 -D DEBUG_JP_PROFILE #-O3


INCLUDES = -I./
INCLUDES = -I${COLPACK_ROOT}/INC
INCLUDES+= -I${COLPACK_ROOT}/SRC/GeneralGraphColoring
INCLUDES+= -I${COLPACK_ROOT}/SRC/BipartiteGraphBicoloring
INCLUDES+= -I${COLPACK_ROOT}/SRC/BipartiteGraphPartialColoring
INCLUDES+= -I${COLPACK_ROOT}/SRC/Utilities
INCLUDES+= -I${COLPACK_ROOT}/SRC/Recovery
INCLUDES+= -I${COLPACK_ROOT}/SRC/SMPGC
INCLUDES+= -I${COLPACK_ROOT}/SRC/PartialD2SMPGC


all: $(EXE)

%.o : %.cpp
	$(COMPILER) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(EXE): $(OBJ)
	$(COMPILER) $^ $(INCLUDES) $(LDFLAGS)  -o $@

clean:
	rm -f $(OBJ) $(EXE)

