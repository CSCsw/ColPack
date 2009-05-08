# Makefile for 'BipartiteGraphBicoloring' directory

include ../common_variables.in

#Header files
HEADERFILES  =  $(COMMON_HEADER_FILES) \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphCore.h \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphInputOutput.h \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphOrdering.h \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphVertexCover.h \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphBicoloring.h \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphBicoloringInterface.h \


# TARGETS
all: BipartiteGraphBicoloring.o \
	BipartiteGraphBicoloringInterface.o \
	BipartiteGraphCore.o \
	BipartiteGraphVertexCover.o \
	BipartiteGraphInputOutput.o \
	BipartiteGraphOrdering.o

BipartiteGraphBicoloring.o: $(HEADERFILES) BipartiteGraphBicoloring.cpp

BipartiteGraphBicoloringInterface.o: $(HEADERFILES) BipartiteGraphBicoloringInterface.cpp

BipartiteGraphCore.o: $(HEADERFILES) BipartiteGraphCore.cpp

BipartiteGraphVertexCover.o: $(HEADERFILES) BipartiteGraphVertexCover.cpp

BipartiteGraphInputOutput.o: $(HEADERFILES) BipartiteGraphInputOutput.cpp

BipartiteGraphOrdering.o: $(HEADERFILES) BipartiteGraphOrdering.cpp

install:

clean:
	@echo "Deleting files ..."
	rm -rf *~ *.o
wipe:
	@echo "Deleting files ..."
	rm -rf *~ *.o *.log
