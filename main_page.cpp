/*! \mainpage ColPack
 *
 * <CENTER><H2>Assefaw H. Gebremedhin, Arijit Tarafdar, Duc Nguyen, Alex Pothen</H2></CENTER> 
 *
 * \section INTRODUCTION
 *
 * ColPack is a package comprising of implementation of algorithms for specialized vertex coloring problems 
 * that arise in sparse derivative computation. It is written in an object-oriented fashion heavily using 
 * the Standard Template Library (STL). It is designed to be simple, modular, extenable and efficient.
 *
 * \section SAMPLE_CODES
 *
 * Sample codes (with comments) that quickly illustrate how ColPack interface functions are used 
 * are available in the directory SampleDriver.<BR>
 * Click on <a href="files.html">"Files"</a> tab and then pick the files you want to look at and click on the [code] link.<BR>
 * <BR>
 * To test (compile) any of these drivers on UNIX:<BR>
 * - Replace "ColPack root"/Main/Main.cpp with that driver. 
 * - Run "make" in "ColPack root"/ directory. The make command could also be run with parameters: "make EXECUTABLE=(desired name. Optional, default name is ColPack) INSTALL_DIR=(directory where the compiled program will be placed. Optional, default dir is ./)". On multi-processors computer, add flag "-j" for faster result. 
 *
 * \section DOWNLOAD 
 *
 * <a href="ColPack.zip"> ColPack.zip</a><BR>
 * <a href="ColPack.tar.gz"> ColPack.tar.gz</a><BR>
 * <a href="MeTiS_Collection.zip"> Graph Collection in MeTis format</a><BR>
 * <a href="MM_Collection.zip"> Graph Collection in Matrix Market format</a><BR>
 * To decompress .zip files on UNIX, run "unzip (targeted .zip file)"<BR>
 *
 * \section CONTACT 
 *
 * Email Assefaw Gebremedhin at <a href="mailto:assefaw@cs.odu.edu?cc=dnguyen@cs.odu.edu">assefaw [at] cs [dot] odu [dot] edu </a>.
 * 
 */

/** @defgroup group1 Classes for Graphs
 Based on functionalities, the general graph coloring part of ColPack is divided into five classes -
GraphCore, GraphInputOutput, GraphOrdering, GraphColoring and GraphColoringInterface. In the
methods described below if no return type is specifed, it would be an int by default. Most ColPack
methods return TRUE on success, which has an integer value of 1.
 */

/**
 *  @defgroup group2 Classes for Bipartite Graphs
 */

/** @defgroup group21 Classes for Bipartite Graphs Partial Coloring
 *  @ingroup group2
 */

/** @defgroup group22 Classes for Bipartite Graphs BiColoring
 *  @ingroup group2
 */

/** @defgroup group5 Recovery Classes
 */

/** @defgroup group4 Auxiliary Classes
 */

