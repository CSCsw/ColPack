# Makefile for TOP directory

include common_variables.in

# Common variables for modification
PACKAGE = ColPack
EXECUTABLE = $(PACKAGE).exe
LIBRARIES = lib$(PACKAGE).a
INSTALL_DIR = .

# Redefine directories
MAIN_DIR = Main
UTILITIES_DIR = Utilities
GRAPHCOLORING_DIR = GraphColoring
BIPARTITEGRAPHBICOLORING_DIR = BipartiteGraphBicoloring
BIPARTITEGRAPHPARTIALCOLORING_DIR = BipartiteGraphPartialColoring
RECOVERY_DIR = Recovery

SUBDIRS = $(MAIN_DIR) $(UTILITIES_DIR) $(GRAPHCOLORING_DIR) $(BIPARTITEGRAPHBICOLORING_DIR) $(BIPARTITEGRAPHPARTIALCOLORING_DIR) $(RECOVERY_DIR)

# TARGETS

all: build-all lib shlib

build-all:
	@echo
	@echo "############################"
	@echo "### BUILDING ALL TARGETS ###"
	@echo "############################"
	@echo \
	
	@for i in $(SUBDIRS) ; do \
	( cd $$i && $(MAKE) ) ; \
	done	\
	
	$(CC_LINKER) $(FLAGS) -o $(EXECUTABLE) \
	$(GRAPHCOLORING_DIR)/GraphCore.o \
	$(GRAPHCOLORING_DIR)/GraphInputOutput.o \
	$(GRAPHCOLORING_DIR)/GraphOrdering.o \
	$(GRAPHCOLORING_DIR)/GraphColoring.o \
	$(GRAPHCOLORING_DIR)/GraphColoringInterface.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphCore.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphInputOutput.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphOrdering.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphVertexCover.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphBicoloring.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphBicoloringInterface.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialOrdering.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialColoring.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialColoringInterface.o \
	$(RECOVERY_DIR)/HessianRecovery.o \
	$(RECOVERY_DIR)/JacobianRecovery1D.o \
	$(RECOVERY_DIR)/JacobianRecovery2D.o \
	$(UTILITIES_DIR)/StringTokenizer.o \
	$(UTILITIES_DIR)/DisjointSets.o \
	$(UTILITIES_DIR)/File.o \
	$(UTILITIES_DIR)/current_time.o \
	$(UTILITIES_DIR)/Pause.o \
	$(UTILITIES_DIR)/Timer.o \
	$(UTILITIES_DIR)/command_line_parameter_processor.o \
	$(UTILITIES_DIR)/extra.o \
	$(MAIN_DIR)/Main.o \
	
	@echo " "
	@echo "Executable Created: $(INSTALL_DIR)/$(EXECUTABLE)"
	@echo " "
	-cp $(EXECUTABLE) $(INSTALL_DIR)

lib:$(GRAPHCOLORING_DIR)/GraphCore.o \
	$(GRAPHCOLORING_DIR)/GraphInputOutput.o \
	$(GRAPHCOLORING_DIR)/GraphOrdering.o \
	$(GRAPHCOLORING_DIR)/GraphColoring.o \
	$(GRAPHCOLORING_DIR)/GraphColoringInterface.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphCore.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphInputOutput.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphOrdering.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphVertexCover.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphBicoloring.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphBicoloringInterface.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialOrdering.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialColoring.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialColoringInterface.o \
	$(RECOVERY_DIR)/HessianRecovery.o \
	$(RECOVERY_DIR)/JacobianRecovery1D.o \
	$(RECOVERY_DIR)/JacobianRecovery2D.o \
	$(UTILITIES_DIR)/StringTokenizer.o \
	$(UTILITIES_DIR)/DisjointSets.o \
	$(UTILITIES_DIR)/File.o \
	$(UTILITIES_DIR)/current_time.o \
	$(UTILITIES_DIR)/Pause.o \
	$(UTILITIES_DIR)/Timer.o \
	$(UTILITIES_DIR)/extra.o \
	
	$(AR) $(LIBFLAGS) $(LIBRARIES) \
	$(GRAPHCOLORING_DIR)/GraphCore.o \
	$(GRAPHCOLORING_DIR)/GraphInputOutput.o \
	$(GRAPHCOLORING_DIR)/GraphOrdering.o \
	$(GRAPHCOLORING_DIR)/GraphColoring.o \
	$(GRAPHCOLORING_DIR)/GraphColoringInterface.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphCore.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphInputOutput.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphOrdering.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphVertexCover.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphBicoloring.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphBicoloringInterface.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialOrdering.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialColoring.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialColoringInterface.o \
	$(RECOVERY_DIR)/HessianRecovery.o \
	$(RECOVERY_DIR)/JacobianRecovery1D.o \
	$(RECOVERY_DIR)/JacobianRecovery2D.o \
	$(UTILITIES_DIR)/StringTokenizer.o \
	$(UTILITIES_DIR)/DisjointSets.o \
	$(UTILITIES_DIR)/File.o \
	$(UTILITIES_DIR)/current_time.o \
	$(UTILITIES_DIR)/Pause.o \
	$(UTILITIES_DIR)/Timer.o \
	$(UTILITIES_DIR)/extra.o \

	$(RANLIB) $(LIBRARIES)
	@echo " "
	@echo "Library Created: $(INSTALL_DIR)/$(LIBRARIES)"
	@echo " "

shlib:$(GRAPHCOLORING_DIR)/GraphCore.o \
	$(GRAPHCOLORING_DIR)/GraphInputOutput.o \
	$(GRAPHCOLORING_DIR)/GraphOrdering.o \
	$(GRAPHCOLORING_DIR)/GraphColoring.o \
	$(GRAPHCOLORING_DIR)/GraphColoringInterface.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphCore.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphInputOutput.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphOrdering.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphVertexCover.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphBicoloring.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphBicoloringInterface.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialOrdering.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialColoring.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialColoringInterface.o \
	$(RECOVERY_DIR)/HessianRecovery.o \
	$(RECOVERY_DIR)/JacobianRecovery1D.o \
	$(RECOVERY_DIR)/JacobianRecovery2D.o \
	$(UTILITIES_DIR)/StringTokenizer.o \
	$(UTILITIES_DIR)/DisjointSets.o \
	$(UTILITIES_DIR)/File.o \
	$(UTILITIES_DIR)/current_time.o \
	$(UTILITIES_DIR)/Pause.o \
	$(UTILITIES_DIR)/Timer.o \
	$(UTILITIES_DIR)/extra.o \

	
	${CXX}	${CXXFLAGS} -shared -Wl,-soname,libcolpack.so -o libcolpack.so \
	$(GRAPHCOLORING_DIR)/GraphCore.o \
	$(GRAPHCOLORING_DIR)/GraphInputOutput.o \
	$(GRAPHCOLORING_DIR)/GraphOrdering.o \
	$(GRAPHCOLORING_DIR)/GraphColoring.o \
	$(GRAPHCOLORING_DIR)/GraphColoringInterface.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphCore.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphInputOutput.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphOrdering.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphVertexCover.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphBicoloring.o \
	$(BIPARTITEGRAPHBICOLORING_DIR)/BipartiteGraphBicoloringInterface.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialOrdering.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialColoring.o \
	$(BIPARTITEGRAPHPARTIALCOLORING_DIR)/BipartiteGraphPartialColoringInterface.o \
	$(RECOVERY_DIR)/HessianRecovery.o \
	$(RECOVERY_DIR)/JacobianRecovery1D.o \
	$(RECOVERY_DIR)/JacobianRecovery2D.o \
	$(UTILITIES_DIR)/StringTokenizer.o \
	$(UTILITIES_DIR)/DisjointSets.o \
	$(UTILITIES_DIR)/File.o \
	$(UTILITIES_DIR)/current_time.o \
	$(UTILITIES_DIR)/Pause.o \
	$(UTILITIES_DIR)/Timer.o \
	$(UTILITIES_DIR)/extra.o \

	@echo " "
	@echo " Shared Library Created: "
	@echo " "

	
all-clean: all clean
	
clean:
	@echo
	@echo "################"
	@echo "### CLEAN UP ###"
	@echo "################"
	@echo \
	
	rm -rf *~ *.o
	@for i in $(SUBDIRS) ; do \
	( cd $$i && $(MAKE) clean) ; \
	done

ship: clean
	-rm -rf *.vcproj* *.ncb *.sln *.suo debug core ColPack.exe libColPack.a
	d2ua
	(cd .. && dz ColPack;chmod 755 ColPack.tar.gz;)

install:
	@echo
	@echo "################################"
	@echo "### INSTALL EXECUTABLE FILES ###"
	@echo "################################"
	@echo \
	
	@for i in $(SUBDIRS) ; do \
	( cd $$i && $(MAKE) install); \
	done

	# create build directory
	mkdir -p build/include
	mkdir -p build/lib

	# copy header files to ./build/include
	@for i in $(SUBDIRS) ; do \
	( cp $$i/*.h build/include); \
	done

	# copy libraries to ./build/lib
	cp libColPack.a build/lib
	cp libcolpack.so build/lib
	



wipe:
	@echo
	@echo "################################################"
	@echo "### WIPE EVERYTHING INCLUDING THE EXECUTABLES ###"
	@echo "################################################"
	@echo \
	
	rm -rf *~ *.o *.log core $(EXECUTABLE) $(LIBRARIES)
	@for i in $(SUBDIRS) ; do \
	( cd $$i && $(MAKE) wipe) ; \
	done

# Rules to build individual targets
Color:
	@cd $(GRAPHCOLORING_DIR) && $(MAKE)
	
Bicolor:
	@cd $(BIPARTITEGRAPHBICOLORING_DIR) && $(MAKE)

PBicolor:
	@cd $(BIPARTITEGRAPHPARTIALCOLORING_DIR) && $(MAKE)

Recovery:
	@cd $(RECOVERY_DIR) && $(MAKE)

Utils:
	@cd $(UTILITIES_DIR) && $(MAKE)

Main:
	@cd $(MAIN_DIR) && $(MAKE)
