# Makefile for 'Recovery' directory

include ../common_variables.in

#Header files
HEADERFILES  =  $(COMMON_HEADER_FILES) \


# TARGETS
all: HessianRecovery.o \
	JacobianRecovery1D.o \
	JacobianRecovery2D.o \


HessianRecovery.o: $(HEADERFILES) HessianRecovery.h HessianRecovery.cpp

JacobianRecovery1D.o: $(HEADERFILES) JacobianRecovery1D.h JacobianRecovery1D.cpp

JacobianRecovery2D.o: $(HEADERFILES) JacobianRecovery2D.h JacobianRecovery2D.cpp

install:

clean:
	@echo "Deleting files ..."
	rm -rf *~ *.o
wipe:
	@echo "Deleting files ..."
	rm -rf *~ *.o *.log
