# Makefile for 'Utilities' directory

include ../common_variables.in

#Header files
HEADERFILES  =  \
	$(MAIN_DIR)/Definitions.h \

# TARGETS
all: command_line_parameter_processor.o \
	DisjointSets.o \
	extra.o \
	File.o \
	current_time.o \
	Pause.o \
	StringTokenizer.o \
	Timer.o

command_line_parameter_processor.o: $(HEADERFILES) command_line_parameter_processor.h command_line_parameter_processor.cpp

DisjointSets.o: $(HEADERFILES) DisjointSets.h DisjointSets.cpp

extra.o: $(HEADERFILES) extra.h extra.cpp

File.o: $(HEADERFILES) File.h File.cpp

current_time.o: $(HEADERFILES) current_time.h current_time.cpp

Pause.o: $(HEADERFILES) Pause.h Pause.cpp

StringTokenizer.o: $(HEADERFILES) StringTokenizer.h StringTokenizer.cpp

Timer.o: $(HEADERFILES) Timer.h Timer.cpp

install:

clean:
	@echo "Deleting files ..."
	rm -rf *~ *.o
wipe:
	@echo "Deleting files ..."
	rm -rf *~ *.o *.log
