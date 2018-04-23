/*************************************************************************
    File Name: SMPGCInterface.h
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:19:24 AM EST
*********************************************************************/
#ifndef SMPGCORDERING_H
#define SMPGCORDERING_H
#include <vector>
#include <iostream>
#include <omp.h>
#include "ColPackHeaders.h" //#include "GraphOrdering.h"
#include "SMPGCCore.h"



using namespace std;

namespace ColPack {

//=============================================================================
// Shared Memeory Parallel (Greedy)/Graph Coloring -> SMPGC
// ----------------------------------------------------------------------------
// 
// SMPGC includes three main algorithms
// * GM's Algorithm: Gebremedhin and Manne[1].
// * IP's Algorithm: Catalyurek Feo Gebremedhin and Halappanavar[2]
// * JP's Algorithm: Jones and Plassmann[3]
// * Luby's Alg
// * JP-LF
// * JP_SL
// ----------------------------------------------------------------------------
// [1] Scalable Parallel Graph Coloring Algorithms
// [2] Grah coloring algorithms for multi-core and massively multithreaded architectures
// [3] A Parallel Graph Coloring Heuristic
//=============================================================================
    



// ============================================================================
// Shared Memory Parallel Greedy/Graph Coloring Ordering wrap
// ============================================================================
class SMPGCOrdering : public SMPGCCore {
public: // construction
    SMPGCOrdering(const string& file_name, const string& fmt, double*iotime, const string& order, double *ordtime);
    virtual ~SMPGCOrdering();
public: // deplete construction
    SMPGCOrdering(SMPGCOrdering&&)=delete;
    SMPGCOrdering(const SMPGCOrdering&)=delete;
    SMPGCOrdering& operator=(SMPGCOrdering&&)=delete;
    SMPGCOrdering& operator=(const SMPGCOrdering&)=delete;
public: // API
    void NaturalOrdering(vector<INT>& vtxs, INT N);
    void RandomOrdering(vector<INT>& vtxs, INT N);
    void local_largest_degree_first_ordering(vector<INT>& vtxs); //LargestDegreeFirstOrdering(vector<INT>& vtxs, INT N);
    void SmallestDegreeLastOrdering(vector<INT>& vtxs, INT N);
    void DynamicLargestDegreeFirstOrdering(vector<INT>& vtxs, INT N);
    void IncidenceDegreeOrdering(vector<INT>& vtxs, INT N);
    void LogOrdering(vector<INT>& vtxs, INT N);
    virtual void dump();
    vector<INT>& ordered_vertex(){ return ordered_vertex_; }

protected: // members
    vector<INT> ordered_vertex_;   

protected: // misc
    virtual void error(const string& s){ fprintf(stderr,"Err in class SMPGCOrdering with msg \"%s\"\n",s.c_str()); exit(1);}
};




}// endof namespace ColPack
#endif

