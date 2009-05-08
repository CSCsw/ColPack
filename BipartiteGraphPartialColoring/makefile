# Makefile for 'BipartiteGraphPartialColoring' directory

include ../common_variables.in

#Header files
HEADERFILES  =  $(COMMON_HEADER_FILES) \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphCore.h \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphInputOutput.h \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialOrdering.h \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialColoring.h \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialColoringInterface.h \


# TARGETS
all: BipartiteGraphPartialColoring.o \
	BipartiteGraphPartialColoringInterface.o \
	BipartiteGraphPartialOrdering.o

BipartiteGraphPartialColoring.o: $(HEADERFILES) BipartiteGraphPartialColoring.cpp

BipartiteGraphPartialColoringInterface.o: $(HEADERFILES) BipartiteGraphPartialColoringInterface.cpp

BipartiteGraphPartialOrdering.o: $(HEADERFILES) BipartiteGraphPartialOrdering.cpp

install:

clean:
	@echo "Deleting files ..."
	rm -rf *~ *.o
wipe:
	@echo "Deleting files ..."
	rm -rf *~ *.o *.log
