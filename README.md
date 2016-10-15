[![Build Status](https://travis-ci.org/CSCsw/ColPack.svg?branch=master)](https://travis-ci.org/CSCsw/ColPack)

ColPack's Doxygen documentation is available here:
http://cscapes.cs.purdue.edu/coloringpage/software.htm

ColPack's project home page:
http://cscapes.cs.purdue.edu/coloringpage/

# ColPack 

ColPack is a package comprising of implementations of algorithms for the specialized vertex coloring problems discussed in the previous section as well as algorithms for a variety of related supporting tasks in derivative computation.

### Coloring capabilities

the table below gives a quick summary of all the coloring problems (on general and bipartite graphs) supported by ColPack.

| General Graph Coloring | Bipartite Graph one-sided coloring | Bipartite Graph Bicoloring |  
| ---- | ----------------- | -------------------|  
| Distance 1 coloring  | Partial distance-2 coloring  | Star bicoloring |  
| Distance 2 coloring | Partial distance-2 coloring  |   |  
| Star coloring |      |   |  
| Acyclic coloring   | |
|  Restricted star coloring| |
|  Triangular coloring| |

All of the coloring problems listed in the above table are NP-hard. Their corresponding algorithms in ColPack are *greedy* heuristics in the sense that the algorithms progressively extend a partial coloring by processing one vertex at a time, in some order, in each step assigning a vertex the smallest allowable color. Listed beneath each coloring problem in the table is the complexity of the corresponding algorithm in ColPack. In the cases where ColPack has multiple algorithms for a problem (these are designated by the superscript †), the complexity expression corresponds to that of the fastest algorithm. In the complexity expressions,

*the complexity of the corresponding algorithm can be found here [ColPack's project](http://cscapes.cs.purdue.edu/coloringpage/software.htm)*
	


### Ordering techniques

The order in which vertices are processed in a greedy coloring algorithm determines the number of colors used by the algorithm. ColPack has implementations of various effective ordering techniques for each of the supported coloring problems. These are summarized in the table below.

| General Graph Coloring | Bipartite Graph one-sided coloring | Bipartite Graph Bicoloring | 
|---|---|---|
| Natural          | Column Natural                     | Natural                    |
| Largest First    | Column Largest First               | Largest First              |
| Smallest Last    | Column Smallest Last               | Smallest Last              |
| Incidence Degree | Column Incidence Degree            | Incidence Degree           |
| Dynamic Largest First           | Row Natural         | Dynamic Largest First      |
| Distance-2 Largest First        | Row Largest First   | Selective Largest First    |
| Distance-2 Smallest Last        | Row Smallest Last   | Selective Smallest Last    |
| Distance-2 Incidence Degree     | Row Incidence Degree| Selective Incidence Degree |
| Distance-2 Dynamic Largest First|                     |  


### Recovery routines

Besides coloring and ordering capabilities, ColPack also has routines for recovering the numerical values of the entries of a derivative matrix from a compressed representation. In particular the following reconstruction routines are currently available:

* Recovery routines for direct (via star coloring ) and substitution-based (via acyclic coloring) Hessian computation
* Recovery routines for unidirectional, direct Jacobian computation (via column-wise or row-wise distance-2 coloring)
* Recovery routines for bidirectional, direct Jacobian computation via star bicoloring


### Graph construction routines

Finally, as a supporting functionality, ColPack has routines for constructing bipartite graphs (for Jacobians) and adjacency graphs (for Hessians) from files specifying matrix sparsity structures in various formats, including Matrix Market, Harwell-Boeing and MeTis.

### ColPack : organization
ColPack is written in an object-oriented fashion in C++ heavily using the Standard Template Library (STL).  It is designed to be simple, modular, extensible and efficient. Figure 1 below gives an overview of the structure of the major classes of ColPack. 

![ColPack Organization](http://cscapes.cs.purdue.edu/coloringpage/software_files/ColPack_structure_2.png)  


Ubuntu Build Instructions
=========================
In the `ColPack` directory run the following:

    autoreconf -vif
    ./configure --prefix=/path/to/install/
    make -j 4   #Where "4" is the number of cores on your machine
    make install


