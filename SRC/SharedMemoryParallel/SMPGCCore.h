/*************************************************************************
    File Name: SMPGCInterface.h
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:19:24 AM EST
*********************************************************************/
#ifndef SMPGCCORE_H
#define SMPGCCORE_H
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
    INT    nodes() const { return ia_.empty()?0:(ia_.size()-1); }
    double avgDeg() const { return avgDeg_; }
    INT    maxDeg() const { return maxDeg_; }
    INT    minDeg() const { return minDeg_; }
    vector<INT>&   CSRia() { return ia_; }
    vector<INT>&   CSRja() { return ja_; }
    vector<double>& CSRa() { return  a_; }
    INT*   CSRiaPtr() { return &ia_[0]; }
    INT*   CSRjaPtr() { return &ja_[0]; }
    double* CSRaPtr() { return & a_[0]; }
    virtual void dump();

protected: // implements
    virtual void do_read_MM_struct(const string& fname, vector<INT>&vi, vector<INT>&vj, INT*p_maxdeg, INT*p_mindeg, double *p_avgdeg, double*iotime);
    virtual void do_read_Binary_struct(const string& fname, vector<INT>&vi, vector<INT>&vj, INT *p_maxdeg, INT*p_mindeg, double*p_avgdeg, double*iotime);
    virtual void do_write_Binary_struct(const string& fname, vector<INT>&vi, vector<INT>&vj, double*iotime);

protected:
    // CSR format, using Intel MKL naming
    vector<INT>    ia_; //known as verPtr; size: graph size + 1
    vector<INT>    ja_; //known as verVal; size: nnz
    vector<double> a_;  //known as nzval;  size: nnz

    INT    maxDeg_;
    INT    minDeg_;
    double avgDeg_;
protected: // misc
    virtual void error(const string& s){ fprintf(stderr,"Err in class SMPGCCore with msg \"%s\"\n",s.c_str()); exit(1);}
};



}// endof namespace ColPack
#endif

