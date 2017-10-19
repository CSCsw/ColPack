AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

#if ENABLE_OPENMP 
AM_CXXFLAGS = -fopenmp
#endif

bin_PROGRAMS = ColPack.out
ColPack_SOURCES =../Examples/example1/Main.cpp



AM_CPPFLAGS =  $(COLPACK_COMMON_FLAGS) \
		-I../Include  \
		-I../Source/Utilities \
		-I../Source/BipartiteGraphPartialColoring \
		-I../Source/BipartiteGraphBicoloring \
		-I../Source/GeneralGraphColoring 
AM_LDFLAGS = $(COLPACK_COMMON_FLAGS)


LDADD = libColPack.la
AM_DEFAULT_SOURCE_EXT = .cpp
lib_LTLIBRARIES = libColPack.la

#libColPackincludedir = .
#libColPackinclude_HEADERS =\
pkginclude_HEADERS = \
			../Source/Utilities/CoutLock.h \
			../Source/Utilities/command_line_parameter_processor.h  \
			../Source/Utilities/File.h \
			../Source/Utilities/DisjointSets.h \
			../Source/Utilities/current_time.h \
			../Source/Utilities/mmio.h \
			../Source/Utilities/Pause.h  \
			../Source/Utilities/MatrixDeallocation.h \
			../Source/Utilities/Timer.h  \
			../Source/Utilities/StringTokenizer.h \
			../Source/Utilities/extra.h \
			../Source/Utilities/stat.h  \
			../Source/BipartiteGraphPartialColoring/BipartiteGraphPartialColoringInterface.h \
			../Source/BipartiteGraphPartialColoring/BipartiteGraphPartialOrdering.h \
			../Source/BipartiteGraphPartialColoring/BipartiteGraphPartialColoring.h \
			../Source/BipartiteGraphBicoloring/BipartiteGraphBicoloringInterface.h \
			../Source/BipartiteGraphBicoloring/BipartiteGraphVertexCover.h \
			../Source/BipartiteGraphBicoloring/BipartiteGraphOrdering.h \
			../Source/BipartiteGraphBicoloring/BipartiteGraphInputOutput.h \
			../Source/BipartiteGraphBicoloring/BipartiteGraphBicoloring.h \
			../Source/BipartiteGraphBicoloring/BipartiteGraphCore.h \
			../Source/GeneralGraphColoring/GraphColoringInterface.h \
			../Source/GeneralGraphColoring/GraphInputOutput.h \
			../Source/GeneralGraphColoring/GraphColoring.h \
			../Source/GeneralGraphColoring/GraphOrdering.h \
			../Source/GeneralGraphColoring/GraphCore.h \
			../Include/Definitions.h \
			../Include/ColPackHeaders.h

libColPack_la_SOURCES = \
			../Source/Utilities/CoutLock.cpp \
			../Source/Utilities/command_line_parameter_processor.cpp \
			../Source/Utilities/File.cpp \
			../Source/Utilities/DisjointSets.cpp \
			../Source/Utilities/current_time.cpp \
			../Source/Utilities/mmio.cpp \
			../Source/Utilities/Pause.cpp \
			../Source/Utilities/MatrixDeallocation.cpp \
			../Source/Utilities/Timer.cpp \
			../Source/Utilities/StringTokenizer.cpp \
			../Source/Utilities/extra.cpp \
			../Source/Utilities/stat.cpp \
			../Source/BipartiteGraphPartialColoring/BipartiteGraphPartialOrdering.cpp \
			../Source/BipartiteGraphPartialColoring/BipartiteGraphPartialColoring.cpp \
			../Source/BipartiteGraphPartialColoring/BipartiteGraphPartialColoringInterface.cpp \
			../Source/BipartiteGraphBicoloring/BipartiteGraphInputOutput.cpp \
			../Source/BipartiteGraphBicoloring/BipartiteGraphBicoloring.cpp \
			../Source/BipartiteGraphBicoloring/BipartiteGraphVertexCover.cpp \
			../Source/BipartiteGraphBicoloring/BipartiteGraphCore.cpp \
			../Source/BipartiteGraphBicoloring/BipartiteGraphBicoloringInterface.cpp \
			../Source/BipartiteGraphBicoloring/BipartiteGraphOrdering.cpp \
			../Source/GeneralGraphColoring/GraphCore.cpp \
			../Source/GeneralGraphColoring/GraphColoringInterface.cpp \
			../Source/GeneralGraphColoring/GraphInputOutput.cpp \
			../Source/GeneralGraphColoring/GraphOrdering.cpp \
			../Source/GeneralGraphColoring/GraphColoring.cpp 




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




