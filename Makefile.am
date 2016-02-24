ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
COLPACK_COMMON_FLAGS = $(EXTRA_FLAGS)  -O3

AM_CPPFLAGS =  $(COLPACK_COMMON_FLAGS) \
		-I$(top_srcdir)/Main \
		-I$(top_srcdir)/Utilities \
		-I$(top_srcdir)/BipartiteGraphPartialColoring \
		-I$(top_srcdir)/BipartiteGraphBicoloring \
		-I$(top_srcdir)/GraphColoring \
		-I$(top_srcdir)/Recovery
AM_LDFLAGS = $(COLPACK_COMMON_FLAGS)

if ENABLE_OPENMP
AM_CXXFLAGS = -fopenmp
endif

LDADD = libColPack.la
AM_DEFAULT_SOURCE_EXT = .cpp

noinst_PROGRAMS = ColPack
lib_LTLIBRARIES = libColPack.la
pkginclude_HEADERS = \
			Utilities/CoutLock.h Utilities/command_line_parameter_processor.h  \
			Utilities/File.h Utilities/DisjointSets.h \
			Utilities/current_time.h Utilities/mmio.h Utilities/Pause.h  \
			Utilities/MatrixDeallocation.h Utilities/Timer.h  \
			Utilities/StringTokenizer.h Utilities/extra.h Utilities/stat.h  \
			BipartiteGraphPartialColoring/BipartiteGraphPartialColoringInterface.h \
			BipartiteGraphPartialColoring/BipartiteGraphPartialOrdering.h \
			BipartiteGraphPartialColoring/BipartiteGraphPartialColoring.h \
			BipartiteGraphBicoloring/BipartiteGraphBicoloringInterface.h \
			BipartiteGraphBicoloring/BipartiteGraphVertexCover.h \
			BipartiteGraphBicoloring/BipartiteGraphOrdering.h \
			BipartiteGraphBicoloring/BipartiteGraphInputOutput.h \
			BipartiteGraphBicoloring/BipartiteGraphBicoloring.h \
			BipartiteGraphBicoloring/BipartiteGraphCore.h \
			GraphColoring/GraphColoringInterface.h \
			GraphColoring/GraphInputOutput.h \
			GraphColoring/GraphColoring.h \
			GraphColoring/GraphOrdering.h \
			GraphColoring/GraphCore.h \
			Recovery/JacobianRecovery2D.h \
			Recovery/JacobianRecovery1D.h \
			Recovery/HessianRecovery.h \
			Recovery/RecoveryCore.h \
			Main/Definitions.h \
			Main/ColPackHeaders.h

libColPack_la_SOURCES = \
			Utilities/CoutLock.cpp Utilities/command_line_parameter_processor.cpp \
			Utilities/File.cpp Utilities/DisjointSets.cpp \
			Utilities/current_time.cpp Utilities/mmio.cpp Utilities/Pause.cpp \
			Utilities/MatrixDeallocation.cpp Utilities/Timer.cpp \
			Utilities/StringTokenizer.cpp Utilities/extra.cpp Utilities/stat.cpp \
			BipartiteGraphPartialColoring/BipartiteGraphPartialOrdering.cpp \
			BipartiteGraphPartialColoring/BipartiteGraphPartialColoring.cpp \
			BipartiteGraphPartialColoring/BipartiteGraphPartialColoringInterface.cpp \
			BipartiteGraphBicoloring/BipartiteGraphInputOutput.cpp \
			BipartiteGraphBicoloring/BipartiteGraphBicoloring.cpp \
			BipartiteGraphBicoloring/BipartiteGraphVertexCover.cpp \
			BipartiteGraphBicoloring/BipartiteGraphCore.cpp \
			BipartiteGraphBicoloring/BipartiteGraphBicoloringInterface.cpp \
			BipartiteGraphBicoloring/BipartiteGraphOrdering.cpp \
			GraphColoring/GraphCore.cpp \
			GraphColoring/GraphColoringInterface.cpp \
			GraphColoring/GraphInputOutput.cpp \
			GraphColoring/GraphOrdering.cpp \
			GraphColoring/GraphColoring.cpp \
			Recovery/JacobianRecovery1D.cpp \
			Recovery/RecoveryCore.cpp \
			Recovery/JacobianRecovery2D.cpp \
			Recovery/HessianRecovery.cpp

ColPack_SOURCES = \
			Main/Main.cpp



examplesdir = ${prefix}/examples
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
