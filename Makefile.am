ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
COLPACK_COMMON_FLAGS = $(EXTRA_FLAGS)  -O3

AM_CPPFLAGS =  $(COLPACK_COMMON_FLAGS) \
		-I$(top_srcdir)/INC \
		-I$(top_srcdir)/SRC/Utilities \
		-I$(top_srcdir)/SRC/BipartiteGraphPartialColoring \
		-I$(top_srcdir)/SRC/BipartiteGraphBicoloring \
		-I$(top_srcdir)/SRC/GeneralGraphColoring \
		-I$(top_srcdir)/SRC/Recovery
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
			SRC/Utilities/CoutLock.h SRC/Utilities/command_line_parameter_processor.h  \
			SRC/Utilities/File.h SRC/Utilities/DisjointSets.h \
			SRC/Utilities/current_time.h SRC/Utilities/mmio.h SRC/Utilities/Pause.h  \
			SRC/Utilities/MatrixDeallocation.h SRC/Utilities/Timer.h  \
			SRC/Utilities/StringTokenizer.h SRC/Utilities/extra.h SRC/Utilities/stat.h  \
			SRC/BipartiteGraphPartialColoring/BipartiteGraphPartialColoringInterface.h \
			SRC/BipartiteGraphPartialColoring/BipartiteGraphPartialOrdering.h \
			SRC/BipartiteGraphPartialColoring/BipartiteGraphPartialColoring.h \
			SRC/BipartiteGraphBicoloring/BipartiteGraphBicoloringInterface.h \
			SRC/BipartiteGraphBicoloring/BipartiteGraphVertexCover.h \
			SRC/BipartiteGraphBicoloring/BipartiteGraphOrdering.h \
			SRC/BipartiteGraphBicoloring/BipartiteGraphInputOutput.h \
			SRC/BipartiteGraphBicoloring/BipartiteGraphBicoloring.h \
			SRC/BipartiteGraphBicoloring/BipartiteGraphCore.h \
			SRC/GeneralGraphColoring/GraphColoringInterface.h \
			SRC/GeneralGraphColoring/GraphInputOutput.h \
			SRC/GeneralGraphColoring/GraphColoring.h \
			SRC/GeneralGraphColoring/GraphOrdering.h \
			SRC/GeneralGraphColoring/GraphCore.h \
			SRC/Recovery/JacobianRecovery2D.h \
			SRC/Recovery/JacobianRecovery1D.h \
			SRC/Recovery/HessianRecovery.h \
			SRC/Recovery/RecoveryCore.h \
			INC/Definitions.h \
			INC/ColPackHeaders.h

libColPack_la_SOURCES = \
			SRC/Utilities/CoutLock.cpp SRC/Utilities/command_line_parameter_processor.cpp \
			SRC/Utilities/File.cpp SRC/Utilities/DisjointSets.cpp \
			SRC/Utilities/current_time.cpp SRC/Utilities/mmio.cpp SRC/Utilities/Pause.cpp \
			SRC/Utilities/MatrixDeallocation.cpp SRC/Utilities/Timer.cpp \
			SRC/Utilities/StringTokenizer.cpp SRC/Utilities/extra.cpp SRC/Utilities/stat.cpp \
			SRC/BipartiteGraphPartialColoring/BipartiteGraphPartialOrdering.cpp \
			SRC/BipartiteGraphPartialColoring/BipartiteGraphPartialColoring.cpp \
			SRC/BipartiteGraphPartialColoring/BipartiteGraphPartialColoringInterface.cpp \
			SRC/BipartiteGraphBicoloring/BipartiteGraphInputOutput.cpp \
			SRC/BipartiteGraphBicoloring/BipartiteGraphBicoloring.cpp \
			SRC/BipartiteGraphBicoloring/BipartiteGraphVertexCover.cpp \
			SRC/BipartiteGraphBicoloring/BipartiteGraphCore.cpp \
			SRC/BipartiteGraphBicoloring/BipartiteGraphBicoloringInterface.cpp \
			SRC/BipartiteGraphBicoloring/BipartiteGraphOrdering.cpp \
			SRC/GeneralGraphColoring/GraphCore.cpp \
			SRC/GeneralGraphColoring/GraphColoringInterface.cpp \
			SRC/GeneralGraphColoring/GraphInputOutput.cpp \
			SRC/GeneralGraphColoring/GraphOrdering.cpp \
			SRC/GeneralGraphColoring/GraphColoring.cpp \
			SRC/Recovery/JacobianRecovery1D.cpp \
			SRC/Recovery/RecoveryCore.cpp \
			SRC/Recovery/JacobianRecovery2D.cpp \
			SRC/Recovery/HessianRecovery.cpp

ColPack_SOURCES = \
			APP/Main/Main.cpp



examplesdir = ${prefix}/examples
basic_examplesdir = ${examplesdir}/Basic
basic_examples_PROGRAMS = \
			SRC/SampleDrivers/Basic/color_bipartite_graph_using_BipartiteGraphBicoloringInterface \
			SRC/SampleDrivers/Basic/color_bipartite_graph_using_BipartiteGraphPartialColoringInterface \
			SRC/SampleDrivers/Basic/color_graph_using_GraphColoringInterface \
			SRC/SampleDrivers/Basic/Generate_seed_matrix_for_Hessian \
			SRC/SampleDrivers/Basic/Generate_seed_matrix_for_Jacobian

TESTS = ${basic_examples_PROGRAMS}

if EXAMPLES
ADIC_examplesdir = ${examplesdir}/Matrix_Compression_and_Recovery/ADIC
ADIC_examples_PROGRAMS = \
			SRC/SampleDrivers/Matrix_Compression_and_Recovery/ADIC/01_Column_compression_and_recovery_for_Jacobian_return_ADIC_Format

ADOLC_examplesdir = ${examplesdir}/Matrix_Compression_and_Recovery/ADOL-C
ADOLC_examples_PROGRAMS = \
			SRC/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/01_Column_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format \
			SRC/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/02_Column_compression_and_recovery_for_Jacobian_return_Coordinate_Format \
			SRC/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/03_Column_compression_and_recovery_for_Jacobian_return_Sparse_Solvers_Format \
			SRC/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/04_Row_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format \
			SRC/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/05_Compression_and_direct_recovery_for_Hessian_return_Row_Compressed_Format \
			SRC/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/06_Compression_and_direct_recovery_for_Hessian_return_Coordinate_Format \
			SRC/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/07_Compression_and_direct_recovery_for_Hessian_return_Sparse_Solvers_Format \
			SRC/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/08_Compression_and_indirect_recovery_for_Hessian_return_Row_Compressed_Format \
			SRC/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/09_Bidirectional_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format \
			SRC/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/10_Column_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format__unmanaged_usermem \
			SRC/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/11_Compression_and_direct_recovery_for_Hessian_return_Row_Compressed_Format__unmanaged_usermem \
			SRC/SampleDrivers/Matrix_Compression_and_Recovery/ADOL-C/12_Bidirectional_compression_and_recovery_for_Jacobian_return_Row_Compressed_Format__unmanaged_usermem

CSR_examplesdir = ${examplesdir}/Matrix_Compression_and_Recovery/CSR
CSR_examples_PROGRAMS = \
			SRC/SampleDrivers/Matrix_Compression_and_Recovery/CSR_input/01_Column_compression_and_recovery_for_Jacobian_CSR_input_return_Row_Compressed_Format
endif
