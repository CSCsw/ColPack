ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
COLPACK_COMMON_FLAGS = $(EXTRA_FLAGS)  -O3

AM_CPPFLAGS =  $(COLPACK_COMMON_FLAGS) \
		-I$(top_srcdir)/inc \
		-I$(top_srcdir)/src/Utilities \
		-I$(top_srcdir)/src/BipartiteGraphPartialColoring \
		-I$(top_srcdir)/src/BipartiteGraphBicoloring \
		-I$(top_srcdir)/src/GeneralGraphColoring \
		-I$(top_srcdir)/src/SMPGC \
		-I$(top_srcdir)/src/PartialD2SMPGC \
		-I$(top_srcdir)/src/Recovery
AM_LDFLAGS = $(COLPACK_COMMON_FLAGS)

AM_CXXFLAGS = -std=c++11
if ENABLE_OPENMP
AM_CXXFLAGS += -fopenmp 
endif

LDADD = libColPack.la
AM_DEFAULT_SOURCE_EXT = .cpp

noinst_PROGRAMS = ColPack
lib_LTLIBRARIES = libColPack.la
pkginclude_HEADERS = \
			src/Utilities/CoutLock.h \
			src/Utilities/command_line_parameter_processor.h  \
			src/Utilities/File.h \
			src/Utilities/DisjointSets.h \
			src/Utilities/current_time.h \
			src/Utilities/mmio.h \
			src/Utilities/Pause.h  \
			src/Utilities/MatrixDeallocation.h \
			src/Utilities/Timer.h  \
			src/Utilities/StringTokenizer.h \
			src/Utilities/extra.h \
			src/Utilities/stat.h  \
			src/BipartiteGraphPartialColoring/BipartiteGraphPartialColoringInterface.h \
			src/BipartiteGraphPartialColoring/BipartiteGraphPartialOrdering.h \
			src/BipartiteGraphPartialColoring/BipartiteGraphPartialColoring.h \
			src/BipartiteGraphBicoloring/BipartiteGraphBicoloringInterface.h \
			src/BipartiteGraphBicoloring/BipartiteGraphVertexCover.h \
			src/BipartiteGraphBicoloring/BipartiteGraphOrdering.h \
			src/BipartiteGraphBicoloring/BipartiteGraphInputOutput.h \
			src/BipartiteGraphBicoloring/BipartiteGraphBicoloring.h \
			src/BipartiteGraphBicoloring/BipartiteGraphCore.h \
			src/GeneralGraphColoring/GraphColoringInterface.h \
			src/GeneralGraphColoring/GraphInputOutput.h \
			src/GeneralGraphColoring/GraphColoring.h \
			src/GeneralGraphColoring/GraphOrdering.h \
			src/GeneralGraphColoring/GraphCore.h \
			src/Recovery/JacobianRecovery2D.h \
			src/Recovery/JacobianRecovery1D.h \
			src/Recovery/HessianRecovery.h \
			src/Recovery/RecoveryCore.h \
			inc/Definitions.h \
			inc/ColPackHeaders.h

pkginclude_HEADERS += src/SMPGC/SMPGC.h \
		      src/SMPGC/SMPGCGraph.h \
		      src/SMPGC/SMPGCOrdering.h \
		      src/SMPGC/SMPGCColoring.h

pkginclude_HEADERS += src/PartialD2SMPGC/PD2SMPGC.h \
		      src/PartialD2SMPGC/PD2SMPGCOrdering.h \
		      src/PartialD2SMPGC/PD2SMPGCColoring.h 



libColPack_la_SOURCES = \
			src/Utilities/CoutLock.cpp \
			src/Utilities/command_line_parameter_processor.cpp \
			src/Utilities/File.cpp\
			src/Utilities/DisjointSets.cpp \
			src/Utilities/current_time.cpp \
			src/Utilities/mmio.cpp \
			src/Utilities/Pause.cpp \
			src/Utilities/MatrixDeallocation.cpp \
			src/Utilities/Timer.cpp \
			src/Utilities/StringTokenizer.cpp \
			src/Utilities/extra.cpp \
			src/Utilities/stat.cpp \
			src/BipartiteGraphPartialColoring/BipartiteGraphPartialOrdering.cpp \
			src/BipartiteGraphPartialColoring/BipartiteGraphPartialColoring.cpp \
			src/BipartiteGraphPartialColoring/BipartiteGraphPartialColoringInterface.cpp \
			src/BipartiteGraphBicoloring/BipartiteGraphInputOutput.cpp \
			src/BipartiteGraphBicoloring/BipartiteGraphBicoloring.cpp \
			src/BipartiteGraphBicoloring/BipartiteGraphVertexCover.cpp \
			src/BipartiteGraphBicoloring/BipartiteGraphCore.cpp \
			src/BipartiteGraphBicoloring/BipartiteGraphBicoloringInterface.cpp \
			src/BipartiteGraphBicoloring/BipartiteGraphOrdering.cpp \
			src/GeneralGraphColoring/GraphCore.cpp \
			src/GeneralGraphColoring/GraphColoringInterface.cpp \
			src/GeneralGraphColoring/GraphInputOutput.cpp \
			src/GeneralGraphColoring/GraphOrdering.cpp \
			src/GeneralGraphColoring/GraphColoring.cpp \
			src/Recovery/JacobianRecovery1D.cpp \
			src/Recovery/RecoveryCore.cpp \
			src/Recovery/JacobianRecovery2D.cpp \
			src/Recovery/HessianRecovery.cpp

libColPack_la_SOURCES += src/SMPGC/SMPGCColoring.cpp \
			 src/SMPGC/SMPGCColoringD1.cpp \
			 src/SMPGC/SMPGCColoringD2.cpp \
			 src/SMPGC/SMPGCColoringHybrid.cpp \
			 src/SMPGC/SMPGC.cpp \
			 src/SMPGC/SMPGCGraph.cpp \
			 src/SMPGC/SMPGCOrdering.cpp

libColPack_la_SOURCES += src/PartialD2SMPGC/PD2SMPGCColoring.cpp \
			 src/PartialD2SMPGC/PD2SMPGCColoringDev.cpp \
			 src/PartialD2SMPGC/PD2SMPGC.cpp \
			 src/PartialD2SMPGC/PD2SMPGCOrdering.cpp




ColPack_SOURCES = \
			build/Example_ColPackAll/Main.cpp



examplesdir = ${prefix}/examples
basic_examplesdir = ${examplesdir}/Basic
basic_examples_PROGRAMS = \
			src/SampleDrivers/Basic/color_bipartite_graph_using_BipartiteGraphBicoloringInterface \
			src/SampleDrivers/Basic/color_bipartite_graph_using_BipartiteGraphPartialColoringInterface \
			src/SampleDrivers/Basic/color_graph_using_GraphColoringInterface \
			src/SampleDrivers/Basic/Generate_seed_matrix_for_Hessian \
			src/SampleDrivers/Basic/Generate_seed_matrix_for_Jacobian

TESTS = ${basic_examples_PROGRAMS}

if EXAMPLES
ADIC_examplesdir = ${examplesdir}/Matrix_Compression_and_Recovery/ADIC
ADIC_examples_PROGRAMS = \
			src/SampleDrivers/Matrix_Compression_and_Recovery/ADIC/01_Column_compression_and_recovery_for_Jacobian_return_ADIC_Format

ADOLC_examplesdir = ${examplesdir}/Matrix_Compression_and_Recovery/ADOL-C
ADOLC_examples_PROGRAMS = \
			src/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/01_Column_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format \
			src/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/02_Column_compression_and_recovery_for_Jacobian_return_Coordinate_Format \
			src/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/03_Column_compression_and_recovery_for_Jacobian_return_Sparse_Solvers_Format \
			src/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/04_Row_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format \
			src/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/05_Compression_and_direct_recovery_for_Hessian_return_Row_Compressed_Format \
			src/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/06_Compression_and_direct_recovery_for_Hessian_return_Coordinate_Format \
			src/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/07_Compression_and_direct_recovery_for_Hessian_return_Sparse_Solvers_Format \
			src/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/08_Compression_and_indirect_recovery_for_Hessian_return_Row_Compressed_Format \
			src/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/09_Bidirectional_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format \
			src/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/10_Column_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format__unmanaged_usermem \
			src/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/11_Compression_and_direct_recovery_for_Hessian_return_Row_Compressed_Format__unmanaged_usermem \
			src/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/12_Bidirectional_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format__unmanaged_usermem

CSR_examplesdir = ${examplesdir}/Matrix_Compression_and_Recovery/CSR
CSR_examples_PROGRAMS = \
			src/SampleDrivers/Matrix_Compression_and_Recovery/CSR_input/01_Column_compression_and_recovery_for_Jacobian_CSR_input_return_Row_Compressed_Format
endif
