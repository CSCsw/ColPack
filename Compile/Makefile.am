ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I ../m4
COLPACK_COMMON_FLAGS = $(EXTRA_FLAGS)  -O3

AM_CPPFLAGS =  $(COLPACK_COMMON_FLAGS) \
		-I$(top_srcdir)/Source/Utilities \
		-I$(top_srcdir)/Source/BipartiteGraphPartialColoring \
		-I$(top_srcdir)/Source/BipartiteGraphBicoloring \
		-I$(top_srcdir)/Source/GraphColoring 
AM_LDFLAGS = $(COLPACK_COMMON_FLAGS)

if ENABLE_OPENMP
AM_CXXFLAGS = -fopenmp
endif

LDADD = libColPack.la
AM_DEFAULT_SOURCE_EXT = .cpp

noinst_PROGRAMS = ColPack
lib_LTLIBRARIES = libColPack.la
ROOT = ..
ROOT_UTL = $(ROOT)/Source/Utilities
ROOT_CPK = $(ROOT)/Source/ColPackColoring
ROOT_INC = $(ROOT)/Include
ROOT_EXP = $(ROOT)/Examples
pkginclude_HEADERS = \
			$(ROOT_UTL)/CoutLock.h \
			$(ROOT_UTL)/command_line_parameter_processor.h  \
			$(ROOT_UTL)/File.h \
			$(ROOT_UTL)/DisjointSets.h \
			$(ROOT_UTL)/current_time.h \
			$(ROOT_UTL)/mmio.h \
			$(ROOT_UTL)/Pause.h  \
			$(ROOT_UTL)/MatrixDeallocation.h \
			$(ROOT_UTL)/Timer.h  \
			$(ROOT_UTL)/StringTokenizer.h \
			$(ROOT_UTL)/extra.h \
			$(ROOT_UTL)/stat.h  \
			$(ROOT_CPK)/BipartiteGraphPartialColoring/BipartiteGraphPartialColoringInterface.h \
			$(ROOT_CPK)/BipartiteGraphPartialColoring/BipartiteGraphPartialOrdering.h \
			$(ROOT_CPK)/BipartiteGraphPartialColoring/BipartiteGraphPartialColoring.h \
			$(ROOT_CPK)/BipartiteGraphBicoloring/BipartiteGraphBicoloringInterface.h \
			$(ROOT_CPK)/BipartiteGraphBicoloring/BipartiteGraphVertexCover.h \
			$(ROOT_CPK)/BipartiteGraphBicoloring/BipartiteGraphOrdering.h \
			$(ROOT_CPK)/BipartiteGraphBicoloring/BipartiteGraphInputOutput.h \
			$(ROOT_CPK)/BipartiteGraphBicoloring/BipartiteGraphBicoloring.h \
			$(ROOT_CPK)/BipartiteGraphBicoloring/BipartiteGraphCore.h \
			$(ROOT_CPK)/GraphColoring/GraphColoringInterface.h \
			$(ROOT_CPK)/GraphColoring/GraphInputOutput.h \
			$(ROOT_CPK)/GraphColoring/GraphColoring.h \
			$(ROOT_CPK)/GraphColoring/GraphOrdering.h \
			$(ROOT_CPK)/GraphColoring/GraphCore.h \
			\
			$(ROOT_INC)/Definitions.h \
			$(ROOT_INC)/ColPackHeaders.h

libColPack_la_SOURCES = \
			$(ROOT_UTL)/CoutLock.cpp \
			$(ROOT_UTL)/command_line_parameter_processor.cpp \
			$(ROOT_UTL)/File.cpp \
			$(ROOT_UTL)/DisjointSets.cpp \
			$(ROOT_UTL)/current_time.cpp \
			$(ROOT_UTL)/mmio.cpp \
			$(ROOT_UTL)/Pause.cpp \
			$(ROOT_UTL)/MatrixDeallocation.cpp \
			$(ROOT_UTL)/Timer.cpp \
			$(ROOT_UTL)/StringTokenizer.cpp \
			$(ROOT_UTL)/extra.cpp \
			$(ROOT_UTL)/stat.cpp \
			$(ROOT_CPL)/BipartiteGraphPartialColoring/BipartiteGraphPartialOrdering.cpp \
			$(ROOT_CPL)/BipartiteGraphPartialColoring/BipartiteGraphPartialColoring.cpp \
			$(ROOT_CPL)/BipartiteGraphPartialColoring/BipartiteGraphPartialColoringInterface.cpp \
			$(ROOT_CPL)/BipartiteGraphBicoloring/BipartiteGraphInputOutput.cpp \
			$(ROOT_CPL)/BipartiteGraphBicoloring/BipartiteGraphBicoloring.cpp \
			$(ROOT_CPL)/BipartiteGraphBicoloring/BipartiteGraphVertexCover.cpp \
			$(ROOT_CPL)/BipartiteGraphBicoloring/BipartiteGraphCore.cpp \
			$(ROOT_CPL)/BipartiteGraphBicoloring/BipartiteGraphBicoloringInterface.cpp \
			$(ROOT_CPL)/BipartiteGraphBicoloring/BipartiteGraphOrdering.cpp \
			$(ROOT_CPL)/GeneralGraphColoring/GraphCore.cpp \
			$(ROOT_CPL)/GeneralGraphColoring/GraphColoringInterface.cpp \
			$(ROOT_CPL)/GeneralGraphColoring/GraphInputOutput.cpp \
			$(ROOT_CPL)/GeneralGraphColoring/GraphOrdering.cpp \
			$(ROOT_CPL)/GeneralGraphColoring/GraphColoring.cpp \

ColPack_SOURCES = 
			



examplesdir = $(ROOT_EXP)/SampleDrivers
basic_examplesdir = ${examplesdir}/Basic
basic_examples_PROGRAMS = \
			SampleDrivers/Basic/color_bipartite_graph_using_BipartiteGraphBicoloringInterface \
			SampleDrivers/Basic/color_bipartite_graph_using_BipartiteGraphPartialColoringInterface \
			SampleDrivers/Basic/color_graph_using_GraphColoringInterface \
			SampleDrivers/Basic/Generate_seed_matrix_for_Hessian \
			SampleDrivers/Basic/Generate_seed_matrix_for_Jacobian

TESTS = ${basic_examples_PROGRAMS}

if EXAMPLES
ADIC_examplesdir = ${examplesdir}/Matrix_Compression_and_Recovery/ADIC
ADIC_examples_PROGRAMS = \
			SampleDrivers/Matrix_Compression_and_Recovery/ADIC/01_Column_compression_and_recovery_for_Jacobian_return_ADIC_Format

ADOLC_examplesdir = ${examplesdir}/Matrix_Compression_and_Recovery/ADOL-C
ADOLC_examples_PROGRAMS = \
			SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/01_Column_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format \
			SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/02_Column_compression_and_recovery_for_Jacobian_return_Coordinate_Format \
			SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/03_Column_compression_and_recovery_for_Jacobian_return_Sparse_Solvers_Format \
			SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/04_Row_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format \
			SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/05_Compression_and_direct_recovery_for_Hessian_return_Row_Compressed_Format \
			SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/06_Compression_and_direct_recovery_for_Hessian_return_Coordinate_Format \
			SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/07_Compression_and_direct_recovery_for_Hessian_return_Sparse_Solvers_Format \
			SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/08_Compression_and_indirect_recovery_for_Hessian_return_Row_Compressed_Format \
			SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/09_Bidirectional_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format \
			SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/10_Column_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format__unmanaged_usermem \
			SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/11_Compression_and_direct_recovery_for_Hessian_return_Row_Compressed_Format__unmanaged_usermem \
			SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/12_Bidirectional_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format__unmanaged_usermem

CSR_examplesdir = ${examplesdir}/Matrix_Compression_and_Recovery/CSR
CSR_examples_PROGRAMS = \
			SampleDrivers/Matrix_Compression_and_Recovery/CSR_input/01_Column_compression_and_recovery_for_Jacobian_CSR_input_return_Row_Compressed_Format
endif
