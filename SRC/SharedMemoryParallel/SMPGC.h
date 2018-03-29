/*************************************************************************
    File Name: SMPGCInterface.h
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:19:24 AM EST
*********************************************************************/
#ifndef SMPGCInterface_H
#define SMPGCInterface_H
#include <vector>
#include <unordered_map>
#include <iostream>
#include <omp.h>
#include "ColPackHeaders.h" //#include "GraphOrdering.h"




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
// Shared Memory Parallel Graph Coloring Core 
// ----------------------------------------------------------------------------
// the graphs are stored uisng using CSR format. 
// a, ia, ja. are names inherited from Intel MKL Api
// usually known as non-zero-values,  col-pointers,  col-values
// ============================================================================
class SMPGCCore{
public: // in comman Computer is LP64 Model. change the following in case not. 
    typedef unsigned long long int uint64;
    typedef          long long int  int64;
    typedef unsigned int           uint32;
    typedef          int            int32;
#ifndef INT32
    typedef uint64 UINT;
    typedef int64  INT ;
#else
    typedef uint32 UINT;
    typedef int32  INT ;
#endif
public: // Constructions
    SMPGCCore();
    SMPGCCore(const string& fname, const string& format, double*iotime);
    virtual ~SMPGCCore();
public: // Deplete Constructions
    SMPGCCore(SMPGCCore&&)=delete;
    SMPGCCore(const SMPGCCore&)=delete;
    SMPGCCore& operator=(SMPGCCore&&)=delete; 
    SMPGCCore& operator=(const SMPGCCore&)=delete; 

public: // APIs
    virtual void dump();

protected: // implements
    virtual void do_read_MM_struct(const string& fname, vector<int>&vi, vector<int>&vj, int *p_maxdeg, int*p_mindeg, double*p_avgdeg, double*iotime);
    virtual void do_read_Binary_struct(const string& fname, vector<int>&vi, vector<int>&vj, int *p_maxdeg, int*p_mindeg, double*p_avgdeg, double*iotime);
    virtual void do_write_Binary_struct(const string& fname, vector<int>&vi, vector<int>&vj, double*iotime);

protected:
    // CSR format, using Intel MKL naming
    vector<INT>    ia_; //known as verPtr; size: graph size + 1
    vector<INT>    ja_; //known as verInd; size: nnz
    vector<double> a_;  //known as nzval;  size: nnz

protected: // misc
    virtual void error(const string& s){ fprintf(stderr,"Err in class SMPGCCore with msg \"%s\"\n",s.c_str()); exit(1);}
};


// ============================================================================
// Shared Memory Parallel Greedy/Graph Coloring Ordering wrap
// ============================================================================
class SMPGCOrdering : public SMPGCCore {
public: // construction
    SMPGCOrdering(const string& order, double *ordtime, double*iotime);
    virtual ~SMPGCOrdering();
public: // deplete construction
    SMPGCOrdering(SMPGCOrdering&&)=delete;
    SMPGCOrdering(const SMPGCOrdering&)=delete;
    SMPGCOrdering& operator=(SMPGCOrdering&&)=delete;
    SMPGCOrdering& operator=(const SMPGCordering&)=delete;
public: // API
    void NaturalOrdering(vector<INT>& vtxs);
    void RandomOrdering(vector<INT>& vtxs);
    virtual void dump();
public: // members
    vector<INT> OrderedVtx;   

protected: // misc
    virtual void error(const string& s){ fprintf(stderr,"Err in class SMPGCOrdering with msg \"%s\"\n",s.c_str()); exit(1);}
};


// ============================================================================
// Shared Memeory Parallel Greedy/Graph Coloring
// ============================================================================
class SMPGCInterface : public SMPGCOrdering {
public: // Constructions
    SMPGCInterface(const string& graph_name,double*ordtime, double*iotime, const string&glb_ord);
    virtual ~SMPGCInterface();
        // Deplete constructions
    SMPGCInterface(SMPGCInterface&&)=delete;
    SMPGCInterface(const SMPGCInterface&)=delete;
    SMPGCInterface& operator=(SMPGCInterface&&)=delete;
    SMPGCInterface& operator=(const SMPGCInterface&)=delete;

public: // API
    virtual void dump();
    void Coloring(int nT, int&color, vector<int>&vtxColors, const string&loc_ord, const string& method);
        // Algorithms 
    int D1_OMP_GM(int nT, int&color, vector<int>&vtxColors, const string&loc_ord);
    int D1_OMP_IP(int nT, int&color, vector<int>&vtxColors, const string&loc_ord);
    int D1_OMP_LB(int nT, int&color, vector<int>&vtxColors, const string&loc_ord);
    int D1_OMP_JP(int nT, int&color, vector<int>&vtxColors, const string&loc_ord);
    int D1_OMP_JP_AW_LF(int nT, int&color, vector<int>&vtxColors, const string&loc_ord);
    int D1_OMP_JP_AW_SL(int nT, int&color, vector<int>&vtxColors, const string&loc_ord);

protected: // Utilites
    bool do_verify_colors(int colors,const vector<int>& vc);
    virtual void error(const string& s){ fprintf(stderr,"Err in class SMPGCInterface with msg \"%s\"\n",s.c_str()); exit(1);}
}; // end of class SMPGCInterface


}// endof namespace ColPack
#endif

