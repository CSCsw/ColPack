/*************************************************************************
    File Name: SMPGCInterface.h
    Author: Xin Cheng
    Descriptions: 
    Created Time: Tue 06 Mar 2018 10:19:24 AM EST
*********************************************************************/
#ifndef SMPGCInterface_H
#define SMPGCInterface_H
#include <vector>
#include <omp.h>
#include "ColPackHeaders.h" //#include "GraphOrdering.h"
#include "SMPGCOrdering.h"



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
// Shared Memeory Parallel Greedy/Graph Coloring
// ============================================================================
class SMPGCInterface : public SMPGCOrdering {
public: // Constructions
    SMPGCInterface(const string& graph_name);
    SMPGCInterface(const string& graph_name, const string& fmt, double*iotime, const string&ord, double*ordtime);
    virtual ~SMPGCInterface(){}
        // Deplete constructions
    SMPGCInterface(SMPGCInterface&&)=delete;
    SMPGCInterface(const SMPGCInterface&)=delete;
    SMPGCInterface& operator=(SMPGCInterface&&)=delete;
    SMPGCInterface& operator=(const SMPGCInterface&)=delete;

public: // API
    virtual void dump();
    int Coloring(int nT, const string& method);
        // Algorithms 
    int D1_OMP_GM(int nT, INT&color, vector<INT>&vtxColors);
    int D1_OMP_IP(int nT, INT&color, vector<INT>&vtxColors);
    int D1_OMP_LB(int nT, INT&color, vector<INT>&vtxColors);
    int D1_OMP_JP(int nT, INT&color, vector<INT>&vtxColors);
    int D1_OMP_JP_AW_LF(int nT, INT&color, vector<INT>&vtxColors);
    int D1_OMP_JP_AW_SL(int nT, INT&color, vector<INT>&vtxColors);

protected: // Utilites
    INT cnt_conflict(INT colors,const vector<INT>& vc);
    virtual void error(const string& s){ fprintf(stderr,"Err in class SMPGCInterface with msg \"%s\"\n",s.c_str()); exit(1);}

protected:
    INT num_colors_;
    vector<INT> vertex_color_;

}; // end of class SMPGCInterface


}// endof namespace ColPack
#endif

