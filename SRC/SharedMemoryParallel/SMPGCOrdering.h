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
#include <random>
#include <algorithm>

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

public: // API: global ordering
    void set_global_ordering(const string& order, double*t);
    void global_natural_ordering();
    void global_random_ordering();

public: // API: local ordering
    void set_local_ordering(vector<INT>&vtxs, const string& order, double *t);
    void local_natural_ordering(vector<INT>& vtxs);
    void local_random_ordering (vector<INT>& vtxs);
    void local_largest_degree_first_ordering(vector<INT>& vtxs); 
    void local_largest_degree_first_ordering_emplace(vector<INT>& vtxs, const INT beg, const INT end); 
    void local_smallest_degree_last_ordering(vector<INT>& vtxs);
    void local_smallest_degree_last_ordering_B1a(vector<INT>& vtxs);
    
    //void SmallestDegreeLastOrdering(vector<INT>& vtxs, INT N);
    //void DynamicLargestDegreeFirstOrdering(vector<INT>& vtxs, INT N);
    //void IncidenceDegreeOrdering(vector<INT>& vtxs, INT N);
    //void LogOrdering(vector<INT>& vtxs, INT N);
    
    void set_rseed(const INT x){ mt_.seed(x); }
    
    virtual void dump();
    vector<INT>& ordered_vertex(){ return ordered_vertex_; }

protected: // members
    vector<INT> ordered_vertex_;   
    mt19937_64 mt_;
};




}// endof namespace ColPack
#endif

