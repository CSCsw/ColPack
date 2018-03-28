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
// Shared Memeory Parallel (Greedy) Graph Coloring -> SMPGC
// ----------------------------------------------------------------------------
// 
// SMPGC includes three main algorithms
// * GM's Algorithm: Gebremedhin and Manne[1].
// * IP's Algorithm: Catalyurek Feo Gebremedhin and Halappanavar[2]
// * JP's Algorithm: Jones and Plassmann[3]
//
// ----------------------------------------------------------------------------
// [1] Scalable Parallel Graph Coloring Algorithms
// [2] Grah coloring algorithms for multi-core and massively multithreaded architectures
// [3] A Parallel Graph Coloring Heuristic
//=============================================================================
    class SMPGCInterface : public GraphOrdering {
    public:
        //SMPGCInterface(const string& graph_name);
        SMPGCInterface(const string& graph_name, double*iotime, const string&glb_ord);
        virtual ~SMPGCInterface();
        
        SMPGCInterface(SMPGCInterface&&)=delete;
        SMPGCInterface(const SMPGCInterface&)=delete;
        SMPGCInterface& operator=(SMPGCInterface&&)=delete;
        SMPGCInterface& operator=(const SMPGCInterface&)=delete;

    public: // interface functions and utilites
        int Coloring(int nT, int&color, vector<int>&vtxColors, const string&loc_ord, const string& method);
    protected:
        void do_read_MM_struct(const string& fname, vector<int>&vi, vector<int>&vj, int *p_maxdeg, int*p_mindeg, double*p_avgdeg, double*iotime);
        int  do_local_ordering(vector<int>&orderedmtx,const string& o);
        bool do_verify_colors(int colors,const vector<int>& vc);
        virtual void dump();
        virtual void error(const string&s1);
        virtual void error(const string&s1,const string& s2);

    public: // Algorithms 
        int D1_OMP_GM(int nT, int&color, vector<int>&vtxColors, const string&loc_ord);
        int D1_OMP_IP(int nT, int&color, vector<int>&vtxColors, const string&loc_ord);
        int D1_OMP_LB(int nT, int&color, vector<int>&vtxColors, const string&loc_ord);
        int D1_OMP_JP(int nT, int&color, vector<int>&vtxColors, const string&loc_ord);
        int D1_OMP_JP_AW_LF(int nT, int&color, vector<int>&vtxColors, const string&loc_ord);
        int D1_OMP_JP_AW_SL(int nT, int&color, vector<int>&vtxColors, const string&loc_ord);
     
    }; // end of class SMPGCInterface


}// endof namespace ColPack
#endif

