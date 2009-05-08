# Makefile for 'GraphColoring' directory

include ../common_variables.in

#Header files
HEADERFILES  =  $(COMMON_HEADER_FILES) \
	$(GRAPHCOLORING_DIR)/GraphCore.h \
	$(GRAPHCOLORING_DIR)/GraphInputOutput.h \
	$(GRAPHCOLORING_DIR)/GraphOrdering.h \
	$(GRAPHCOLORING_DIR)/GraphColoring.h \
	$(GRAPHCOLORING_DIR)/GraphColoringInterface.h \


# TARGETS
all: GraphColoring.o \
	GraphColoringInterface.o \
	GraphCore.o \
	GraphInputOutput.o \
	GraphOrdering.o

GraphColoring.o: $(HEADERFILES) GraphColoring.cpp

GraphColoringInterface.o: $(HEADERFILES) GraphColoringInterface.cpp

GraphCore.o: $(HEADERFILES) GraphCore.cpp

GraphInputOutput.o: $(HEADERFILES) GraphInputOutput.cpp

GraphOrdering.o: $(HEADERFILES) GraphOrdering.cpp

install:

clean:
	@echo "Deleting files ..."
	rm -rf *~ *.o
wipe:
	@echo "Deleting files ..."
	rm -rf *~ *.o *.log
