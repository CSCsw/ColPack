# Makefile for 'Main' directory

include ../common_variables.in

#Header files
HEADERFILES  =  $(COMMON_HEADER_FILES) \
	$(GRAPHCOLORING_DIR)/GraphCore.h \
	$(GRAPHCOLORING_DIR)/GraphInputOutput.h \
	$(GRAPHCOLORING_DIR)/GraphOrdering.h \
	$(GRAPHCOLORING_DIR)/GraphColoring.h \
	$(GRAPHCOLORING_DIR)/GraphColoringInterface.h \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphCore.h \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphInputOutput.h \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphOrdering.h \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphVertexCover.h \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphBicoloring.h \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphBicoloringInterface.h \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialOrdering.h \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialColoring.h \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialColoringInterface.h \


# Executables
EXECUTABLE = COLPACK

# TARGETS
all: Main.o

Main.o: $(HEADERFILES) Main.cpp

Main.cpp:
	-mv main.cpp Main.cpp

install:

clean:
	@echo "Deleting files ..."
	rm -rf *~ *.o
wipe:
	@echo "Deleting files ..."
	rm -rf *~ *.o *.log
