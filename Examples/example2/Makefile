# source files
COLPACK_ROOT = ../..
COLPACK_SRC = $(wildcard ${COLPACK_ROOT}/Source/*/*.cpp)
COLPACK_OBJ = $(COLPACK_SRC:%.cpp=%.o)
SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o) $(COLPACK_OBJ)
EXE = ColPack.out


# compiler
COMPILER = g++      # gnu
OMP_FLAG = -fopenmp 

#COMPILER = icc      # intel(R)
#OMP_FLAG = -openmp

# compile flags
CCFLAGS = -Wall -std=c++11 $(OMP_FLAG) -Ofast #-O3
# link flags
LDFLAGS = -Wall -std=c++11 $(OMP_FLAG) -Ofast #-O3


INCLUDES = -I./
INCLUDES+= -I${COLPACK_ROOT}/Include
INCLUDES+= -I${COLPACK_ROOT}/Source/GeneralGraphColoring
INCLUDES+= -I${COLPACK_ROOT}/Source/BipartiteGraphBicoloring
INCLUDES+= -I${COLPACK_ROOT}/Source/BipartiteGraphPartialColoring
INCLUDES+= -I${COLPACK_ROOT}/Source/Utilities


all: $(EXE)

%.o : %.cpp
	$(COMPILER) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(EXE): $(OBJ)
	$(COMPILER) $^ $(INCLUDES) $(LDFLAGS)  -o $@

clean:
	rm -f $(OBJ) $(EXE)

