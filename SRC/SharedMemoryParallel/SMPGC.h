/*************************************************************************
    > File Name: SMPGCDefine.h
    > Author: xc
    > Descriptions: 
    > Created Time: Fri 25 May 2018 04:34:23 PM EDT
 ************************************************************************/

#ifndef SMPGCDEFINE_H
#define SMPGCDEFINE_H

// ============================================================================
// SMPGC: Shared Memory Parallel Graph Coloring
// ----------------------------------------------------------------------------
// **OVERVIEW**
//
// SMPGCCore:                        Graph data, IO
//     |->SMPGCOrdering:             Ordering
//          |-> SMPGCColoring:       D1 Coloring
//          |-> D2SMPGCColoring:     D2 Coloring
//
// ----------------------------------------------------------------------------
// **LIST OF ALGORITHMS**
// * GM's Algorithm: Gebremedhin and Manne[1].
// * IP's Algorithm: Catalyurek Feo Gebremedhin and Halappanavar[2]
// * JP's Algorithm: Jones and Plassmann[3]
// * Luby's Alg
// ...
// ...
// ----------------------------------------------------------------------------
// **LIST OF PAPERS**
// [1] Scalable Parallel Graph Coloring Algorithms
// [2] Grah coloring algorithms for multi-core and massively multithreaded architectures
// [3] A Parallel Graph Coloring Heuristic
// ...
// ...
// ============================================================================

class SMPGC{
//public: // in comman Computer is LP64 Model. change the following in case not. 
    //typedef unsigned long long int uint64;
    //typedef          long long int  int64;
    //typedef unsigned int           uint32;
    //typedef          int            int32;
    //#ifndef INT32
    //    typedef uint64 UINT;
    //    typedef int64  INT ;
    //#else
    //    typedef uint32 UINT;
    //    typedef int32  INT;
    //#endif

public:
    const int RAND_SEED =  12345678;
    const int NUM_HASH  =  4;

    const int JP_HYBER_IMPLEMENT_GM3P = 1;
    const int JP_HYBER_IMPLEMENT_GMMP = 2;
    const int JP_HYBER_IMPLEMENT_GREEDY = 3;
    const int JP_HYBER_IMPLEMENT_STREAM = 4;

    const int _TRUE = 0;

public:
    SMPGC(){};
    ~SMPGC(){};
public:
    SMPGC(SMPGC&&)=delete;
    SMPGC(const SMPGC&)=delete;
    SMPGC& operator=(SMPGC&&)=delete;
    SMPGC& operator=(const SMPGC&)=delete;
};



#endif


